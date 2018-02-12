/***************************************************/
/*                 SuperCom 3.52                   */
/* Windows NT                                      */
/*                                                 */
/* (c) Copyright 1991,97 by ADONTEC Ltd.           */
/***************************************************/

  /* SuperCom version */        
#define SuperComVersion   0x0350



  /* WIN32 Overlapping I/O, added Nov 1996             */
  /* NOTE: define this macro to enable overlapping I/O */
#define __USE_OVERLAPPING__



/* include Development trace output functions */

//#define _DEBUG_	  
 


#define USECOMM      /* include COMM API  */


#undef NO_STRICT


#include <windows.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <process.h> /*##6.2.1996*/

#include "supercom.h"

#include "..\smrtheap.h"

#define THREADSTACK_95  (2*4096)   /* stack size for Win 95 */
#define THREADSTACK_NT  (2*4096)   /* stack size for Win NT */


// EV_RX80FULL is used by this which gives bounds checker an error since I can't find it in the win32 api
DWORD  _EV_ALL =((DWORD)(EV_RXCHAR|EV_TXEMPTY|EV_CTS|EV_DSR|EV_RLSD|EV_BREAK|EV_ERR|EV_RING|EV_RX80FULL));



/*-----------========= Global Data begin ==========---------*/

typedef struct {     /*##6.2.1996*/
      DWORD LastEvent;
      WORD ThreadDone;
	  WORD Suspend;
      HANDLE hThread;
      PUSEREVENTPROC UserEventProc;
}TCOMMTHREADDATA;
typedef TCOMMTHREADDATA  *PCOMMTHREADDATA;

TCOMMTHREADDATA  ThData[MAX_COM];


/* Read cache, used to boost NT/95 Device Driver read function */
#define READCACHEBUFSIZE (1024) 
typedef struct {
	  BYTE Buffer[READCACHEBUFSIZE];
	  DWORD RdPtr;
	  DWORD Count;  
	}TREADCACHE;    
typedef TREADCACHE *PREADCACHE;

TREADCACHE ReadCache[MAX_COM];


#if defined(__USE_OVERLAPPING__)
OVERLAPPED SC_R_OS [MAX_COM];
OVERLAPPED SC_W_OS [MAX_COM];
#endif


CRITICAL_SECTION GlobalCriticalSection;
CRITICAL_SECTION CriticalPortSection[MAX_COM];

TComData ActComData[MAX_COM];

BYTE InitCriticalSect=0;
BYTE PortsInUse=0;

/*-----======= Global Data end ======------*/

void _InitAll(void);
void _InitCritSections(void);
void _DeleteCritSections(void);


void _fastcall _InitCom(BYTE Com);
void _fastcall _StoreState(BYTE Com, PSAVESTATE SaveState);
void _fastcall _RestoreState(BYTE Com, PSAVESTATE SaveState);

void _fastcall _GetCommError(BYTE Com);
void _fastcall _UpdateEvents(BYTE Com, DWORD CommEvents);
DWORD _fastcall _GetMaxBufferSize(BYTE Com, TBufDir BufDir);
BOOLEAN _fastcall _ComRead (BYTE Com, PCHAR RCh);
BOOLEAN _fastcall _ComWrite (BYTE Com, CHAR SCh);
BOOLEAN _fastcall _ComKickOut (BYTE Com, CHAR SCh);
DWORD _fastcall _ComBufCount (BYTE Com, TBufDir BufDir);

DWORD _fastcall osReadFile(BYTE Com, LPCHAR lpBuffer, DWORD dwCount);
DWORD _fastcall osWriteFile(BYTE Com, LPCHAR lpBuffer, DWORD dwCount);

VOID _fastcall ClearReadCacheBuffer(BYTE Com);  /*##8.11.1995*/
VOID _fastcall FillReadCache (BYTE Com, PREADCACHE pReadCache); /*##22.12.1996*/
BOOLEAN _fastcall _CheckRXQueue (BYTE Com, PCHAR RCh, BOOLEAN Remove);
DWORD _fastcall _Crit_GetSetLastEvent(BYTE Com, DWORD NewVal);
BYTE _fastcall _Crit_GetSetErrorState(BYTE Com, BYTE NewVal);



BOOLEAN _fastcall _ComValid(BYTE Com);
void _fastcall GetDCB(BYTE Com, LPDCB lpDCB); /*##20.11.1996*/


 #if defined(_DEBUG_)
 void Trace (LPSTRING point);
 void Trace2 (DWORD val);
 void TraceHex (LPSTRING s, DWORD val);


 BYTE _DebugCom=0;
 WORD TracePoint = 0;
 void DoTrace(LPSTRING point) 
 {
  char c[128];
  char s[128];
  char sec[40];

  TracePoint++;
  wsprintf (c, "TRACE POINT:%d: ", TracePoint);
  wsprintf (s, "%d: %s", ComGetSysTime(), point);
  wsprintf (sec, "COM_%d:", _DebugCom+1);
  WritePrivateProfileString (sec, c, s, "C:\\TRACE.TXT");
 }
 void Trace (LPSTRING point)
 {
#if !defined(_ONLY_EXITCODE_)
   DoTrace(point);
#endif
 }
 void TraceEx (LPSTRING point) 
 {
   DoTrace(point);
 }
 void Trace2 (DWORD val)
 {
 #if !defined(_ONLY_EXITCODE_)
  char s[20];

  ltoa (val, s, 10);
  DoTrace ((LPSTRING) s);
 #endif
 }
 void TraceHex (LPSTRING msg, DWORD val)
 {
 #if !defined(_ONLY_EXITCODE_)
  char s[80];

  wsprintf(s,"%s 0x%4X",msg,val);
  DoTrace ((LPSTRING) s);
 #endif
 }

#else	/* NO DEBUG */

#define Trace(a);
#define Trace2(a);
#define TraceHex(a,b);

#endif



BOOLEAN _fastcall _ComValid(BYTE Com)
{
  if ((Com>LAST_COM)||(PortsInUse==0))
  {
	if ((Com<=LAST_COM)) SCResult(Com)=scINVPORT; /*## 18.3.1996*/
    return(FALSE);
  }

  if (CHANNEL(Com).hCom!=0)
    {
      SCResult(Com)=scOk;
      return(TRUE);
    }
  else
    {
      SCResult(Com)=scINVPORT;
      return(FALSE);
    }
}

void _fastcall GetDCB(BYTE Com, LPDCB lpDCB) /*##20.11.1996*/
{
   memset(lpDCB,0,sizeof(DCB)); /*##3.1.1997*/
   lpDCB->DCBlength = sizeof(DCB);
   GetCommState(CHANNEL(Com).hCom, lpDCB);
}

void _InitAll(void)
{
  memset(ActComData,0,sizeof(ActComData));
  memset(ThData,0,sizeof(ThData)); /*##6.2.1996*/
}
void _fastcall _InitCom(BYTE Com)
{
  memset(&ActComData[Com],0,sizeof(TComData));
  memset(&ThData[Com],0,sizeof(TCOMMTHREADDATA));
}

void _InitCritSections(void)
{
  if (InitCriticalSect==0)
  {
	InitCriticalSect++;
	InitializeCriticalSection(&GlobalCriticalSection);
  }
}

void _DeleteCritSections(void)
{
  if (InitCriticalSect>0) 
  {
	  InitCriticalSect--;
      if (InitCriticalSect==0) 
      {
        DeleteCriticalSection(&GlobalCriticalSection);
      }
  }
}

DWORD _fastcall _Crit_GetSetLastEvent(BYTE Com, DWORD NewVal) /*##3.1.1997*/
{DWORD Res;
  EnterCriticalSection(&CriticalPortSection[Com]);  
    Res = ThData[Com].LastEvent;
    ThData[Com].LastEvent = NewVal;
  LeaveCriticalSection(&CriticalPortSection[Com]);  
  return(Res);
}
BYTE _fastcall _Crit_GetSetErrorState(BYTE Com, BYTE NewVal) /*##3.1.1997*/
{BYTE Res;
  EnterCriticalSection(&CriticalPortSection[Com]);  
    Res = CHANNEL(Com).ErrorState;
    CHANNEL(Com).ErrorState = NewVal;
  LeaveCriticalSection(&CriticalPortSection[Com]);  
  return(Res);
}

void _fastcall _HandleCommError(BYTE Com, DWORD err, LPCOMSTAT cs)
{TComData *p;

     p=&CHANNEL(Com);
     if (err & CE_BREAK) p->ErrorState|=BREAK_DETECT;
     if (err & CE_FRAME) p->ErrorState|=FRAMING_ERROR;
     if (err & CE_RXPARITY) p->ErrorState|=PARITY_ERROR;
     if (err & (CE_OVERRUN | CE_RXOVER)) p->ErrorState|=OVERRUN_ERROR;

			 /* check XON */  /*## 7.5.1996, === ! */
     CHANNEL(Com).fXON=(BOOLEAN)!(cs->fCtsHold||cs->fDsrHold||cs->fRlsdHold||cs->fXoffHold||cs->fXoffSent);
}
void _fastcall _GetCommError(BYTE Com)
{
COMSTAT cs;
DWORD err;

    if (ClearCommError(CHANNEL(Com).hCom, &err, &cs))
    {
      _HandleCommError(Com, err, &cs);
	}
}
void _fastcall _UpdateEvents(BYTE Com, DWORD CommEvents)
{
   if (CommEvents!=0L) /*##19.11.1996, added !=0 to remove compiler warning*/
   {
	      /* update error flags */
     if (CommEvents & EV_BREAK) CHANNEL(Com).ErrorState|=BREAK_DETECT;
     if (CommEvents & EV_ERR) _GetCommError(Com);

			/* update linestatus flags, stored temporally */
     if (CommEvents & (EV_RXCHAR | EV_RX80FULL)) CHANNEL(Com).ErrorState|=RX_READY;
     
	 if (CommEvents & (EV_CTS|EV_DSR|EV_RLSD|EV_RING))
     {   /* Calc Modem Delta */
      BYTE Status=0;

       if (CommEvents & EV_CTS) Status|=DELTA_CTS;
       if (CommEvents & EV_DSR) Status|=DELTA_DSR;
       if (CommEvents & EV_RLSD) Status|=DELTA_DCD;
       if (CommEvents & EV_RING) Status|=DELTA_RI;

       CHANNEL(Com).ModemDelta |= Status;  /*##10.2.1996*/
     }
     
	 if (CommEvents & EV_TXEMPTY) CHANNEL(Com).ErrorState|=TX_READY;
   }
}

/* COMMAPI functions */
WORD COMMAPI ComGetVersion(void)
{
  return(SuperComVersion);
}

PUSEREVENTPROC COMMAPI ComGetSetEventProc(BYTE Com, PUSEREVENTPROC EventProc)
{
PUSEREVENTPROC Res=(PUSEREVENTPROC)NULL;

   if (_ComValid(Com))
   {
	 EnterCriticalSection(&GlobalCriticalSection); /*##30.5.1996*/
	   Res=ThData[Com].UserEventProc;
       ThData[Com].UserEventProc=EventProc;
    LeaveCriticalSection(&GlobalCriticalSection);
   }
   return(Res);
}

/* SuperCom thread. One Thread per port */
/*
#if defined (__BORLANDC__)
void _USERENTRY SComThread(LPVOID lpComParam)
#elif defined (_MSC_VER)
void _cdecl SComThread(LPVOID lpComParam)
#else
DWORD WINAPI SComThread(LPVOID lpComParam)
#endif
*/

#if defined (__USE_OVERLAPPING__)
LPTHREAD_START_ROUTINE SComThread(LPVOID lpComParam)
{
OVERLAPPED osWait;
BOOL WaitEvent;
DWORD dwCommEvent;
DWORD dwRes;
HANDLE hCom;
BYTE Com;

   Com =*(BYTE *)lpComParam;
   hCom=CHANNEL(Com).hCom;


   if (_ComValid(Com))
   {
     memset(&osWait,0,sizeof(OVERLAPPED));

   /* create I/O event */
     osWait.hEvent = CreateEvent(NULL,   /* no security         */
			     FALSE,  /* auto reset req  */
			     FALSE,  /* initial event reset */
			     NULL);  /* no name             */
   	 
	 _Crit_GetSetLastEvent(Com, 0L);

     do
     { 
	   dwCommEvent=0L;

	   WaitEvent = WaitCommEvent(hCom,
				                 (LPDWORD)&dwCommEvent,
				                 (LPOVERLAPPED)&osWait);

	   if (!WaitEvent && (GetLastError()==ERROR_IO_PENDING))
       {
		  Trace("WaitCommEvent - GetOverlapped");
   	      WaitEvent=GetOverlappedResult(hCom, &osWait, &dwRes, TRUE);
       } 

       if (WaitEvent && dwCommEvent)
	    {
	      if (_ComValid(Com))
	      {
  		     TraceHex("Event:",dwCommEvent);
             EnterCriticalSection(&CriticalPortSection[Com]);  /*##3.1.1997*/
               ThData[Com].LastEvent=dwCommEvent;
			   _UpdateEvents(Com, dwCommEvent); /*update SuperCom variables */
             LeaveCriticalSection(&CriticalPortSection[Com]);  /*##3.1.1997*/
		     
			 if (ThData[Com].UserEventProc)
	         {
		        ThData[Com].UserEventProc(Com, dwCommEvent); /* call user proc */
	         }
	      }
	    }
/*     else
	    _Crit_GetSetLastEvent(Com,0L);
*/
     }while (!ThData[Com].ThreadDone);

	 CloseHandle(osWait.hEvent);
   }

   ThData[Com].hThread=0;

   ExitThread(0);
   return(0);
}

#else	/* NO OVERLAPPING */

LPTHREAD_START_ROUTINE SComThread(LPVOID lpComParam)
{
BYTE Com;
BOOL WaitEvent;
DWORD CurEvent;

   Com=*(BYTE *)lpComParam;

   if (_ComValid(Com))
   {
	 _Crit_SetLastEvent(Com, 0L);
	 
     do
     { CurEvent=0L;
      
	   WaitEvent = WaitCommEvent(CHANNEL(Com).hCom,
				                 (LPDWORD)&CurEvent,
				                 (LPOVERLAPPED)NULL);
       if (WaitEvent)
	    {
	      if (CurEvent & _ComValid(Com))
	      {
			_Crit_SetLastEvent(Com, CurEvent);
			_UpdateEvents(Com); /*update SuperCom variables */
	      
		    if (ThData[Com].UserEventProc)
	        {
		      ThData[Com].UserEventProc(Com, CurEvent); /* call user proc */
	        }
	      }
	    }
       else
	    _Crit_SetLastEvent(Com,0L);
	   
     }while (!ThData[Com].ThreadDone);

   }

   ThData[Com].hThread=0;

  ExitThread(0);
  return(0);
}

#endif  /*__USE_OVERLAPPING__*/


BYTE CommA[MAX_COM];  /* global parameter for thread */
void StartCommThread(BYTE Com)  /*##6.2.1996*/
{
DWORD dwTHREADSTACK;
OSVERSIONINFO OSVersionInfo;

   if (ThData[Com].hThread<=0)
   {
     ThData[Com].ThreadDone=FALSE;
     ThData[Com].Suspend=0;
     CommA[Com]=Com;

    /* Calc stack size */
     
	 /* I added this */
	 OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	 GetVersionEx(&OSVersionInfo);
     if (OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
       dwTHREADSTACK=THREADSTACK_NT;
     else
       dwTHREADSTACK=THREADSTACK_95;

/* Start the Com thread */

/*
#if defined (__BORLANDC__)||defined (_MSC_VER)

     ThData[Com].hThread=_beginthread(SComThread,dwTHREADSTACK,(void *)&CommA[Com]);
     if (ThData[Com].hThread==-1L) return;

#else
*/
     { DWORD dwThID;

       ThData[Com].hThread=CreateThread((LPSECURITY_ATTRIBUTES)NULL,
				 dwTHREADSTACK,
			    (LPTHREAD_START_ROUTINE)SComThread,
			    (LPVOID)&CommA[Com],
			    0,
			    (LPDWORD)&dwThID);

       if (ThData[Com].hThread==NULL) return;  /*##19.11.1996, NULL */

     }
/*#endif */
   }
}

void StopCommThread(BYTE Com)  /*##6.2.1996*/
{
   if (ThData[Com].hThread > 0)  /* !=0 and !=-1 */
   {
	 ComThreadState(Com,TRUE); /* resume if needed */
	 ThData[Com].ThreadDone=TRUE;
     ThData[Com].Suspend=0;
/*   SetEvent(SC_C_OS[Com].hEvent);	  signal event and stop WaitSingleObject */
     SetCommMask(CHANNEL(Com).hCom,EV_TXEMPTY);  /* any vallue to kill WaitCommEvent */
   }
  /* if thread is still running */
}

DWORD _fastcall _GetMaxBufferSize(BYTE Com, TBufDir BufDir)
{
COMMPROP cp;
DWORD Res=0L;
  if (!_ComValid(Com)) return(0L);
  GetCommProperties(CHANNEL(Com).hCom, &cp);
  if (BufDir==DIR_INC)
    Res=cp.dwMaxRxQueue;
  else
    if (BufDir==DIR_OUT) Res=cp.dwMaxTxQueue;
  return (Res);
}

VOID _fastcall ClearReadCacheBuffer(BYTE Com)  /*##8.11.1995*/
{
   ReadCache[Com].RdPtr=0L;
   ReadCache[Com].Count=0L;
}

WORD COMMAPI ComThreadState(BYTE Com, WORD NewState)
{
 WORD Res=FALSE;  /* Last Thread state, FALSE==Suspended, TRUE=Running */

	if (_ComValid(Com)) 
	{
	  if (ThData[Com].hThread >0 )
	  {
		  if (NewState)  /* resume */
		  {
			  if (ThData[Com].Suspend==1)
			  {
				Res=FALSE;
				ThData[Com].Suspend=0;
				ResumeThread(ThData[Com].hThread);
			  }
		  }
		  else	  /* suspend */
		  {
			  if (ThData[Com].Suspend==0)
			  {
				Res=TRUE;
				ThData[Com].Suspend=1;
				SuspendThread(ThData[Com].hThread);
			  }
		  }
	  }
	}

	return (Res);
}

void COMMAPI ComSetSCResult(BYTE Com, WORD Res)
{
	SCResult(Com)=Res;
}

WORD COMMAPI ComResult(BYTE Com)
{
WORD tmp;

	 tmp=SCResult(Com);
	 SCResult(Com)=scOk;

  return tmp;
}

BYTE COMMAPI ComGetFlow (BYTE Com)
{
DCB cd;
BYTE Res=SIGNAL_NONE;
  if (!_ComValid(Com)) return(0);


  GetDCB(Com, &cd);

  if (cd.fOutxCtsFlow)
    Res|=SIGNAL_CTS;

  if (cd.fOutxDsrFlow)
    Res|=SIGNAL_DSR;

  if ((cd.fOutX) && (cd.fInX))
    Res|=SIGNAL_XON;

  return(Res);
}

void COMMAPI ComSetFlow (BYTE Com, BYTE Flow)
{
DCB cd;
BYTE nFlow;

  if (!_ComValid(Com)) return;
  GetDCB(Com, &cd);

  nFlow=SIGNAL_NONE;  /* init with NONE */

  if (ComRS485(Com,RS485STATUS)) Flow=SIGNAL_NONE;

  if (Flow & SIGNAL_CTS)
    {
      nFlow|=SIGNAL_CTS;
      cd.fOutxCtsFlow=TRUE;
      cd.fRtsControl=RTS_CONTROL_HANDSHAKE;
    }
  else
    {
      cd.fOutxCtsFlow=FALSE;
      cd.fRtsControl=RTS_CONTROL_ENABLE;
    }

  if (Flow & SIGNAL_DSR)
    {
      nFlow|=SIGNAL_DSR;
      cd.fOutxDsrFlow=TRUE;
      cd.fDtrControl=DTR_CONTROL_HANDSHAKE;
    }
  else
    {
      cd.fOutxDsrFlow=FALSE;
      cd.fDtrControl=DTR_CONTROL_ENABLE;
    }

  if (Flow & SIGNAL_XON)
    {
      nFlow|=SIGNAL_XON;
      cd.fOutX=TRUE;
      cd.fInX =TRUE;
    }
  else
   {
     cd.fOutX=FALSE;
     cd.fInX =FALSE;
   }

  CHANNEL(Com).FlowControl=nFlow; /* store new flow control */
  SetCommState(CHANNEL(Com).hCom, &cd);
}

DWORD COMMAPI ComGetBaudRate (BYTE Com)
{
DCB cd;
  if (!_ComValid(Com)) return(0L);
  GetDCB(Com, &cd);
  return (cd.BaudRate);
}

void COMMAPI ComSetBaudRate (BYTE Com, LONGINT NewBaud)
{
DCB cd;
  if (!_ComValid(Com)) return;

  GetDCB(Com, &cd);
  cd.BaudRate=NewBaud;

  SetCommState(CHANNEL(Com).hCom, &cd);
}

DWORD COMMAPI ComGetMaxBaudRate (BYTE Com)
{
COMMPROP cp;
DWORD Res=0L;
  if (!_ComValid(Com)) return(Res);
  GetCommProperties(CHANNEL(Com).hCom, &cp);
  switch(cp.dwMaxBaud)
  {
    case BAUD_075   :Res=     75L;  break;
    case BAUD_110   :Res=    110L;  break;
    case BAUD_134_5 :Res=    134L;  break;
    case BAUD_150   :Res=    150L;  break;
    case BAUD_300   :Res=    300L;  break;
    case BAUD_600   :Res=    600L;  break;
    case BAUD_1200  :Res=   1200L;  break;
    case BAUD_1800  :Res=   1800L;  break;
    case BAUD_2400  :Res=   2400L;  break;
    case BAUD_4800  :Res=   4800L;  break;
    case BAUD_7200  :Res=   7200L;  break;
    case BAUD_9600  :Res=   9600L;  break;
    case BAUD_14400 :Res= 14400L;   break;
    case BAUD_19200 :Res= 19200L;   break;
    case BAUD_38400 :Res= 38400L;   break;
    case BAUD_56K   :Res= 56000L;   break;
    case BAUD_57600 :Res= 57600L;   break;  /*##20.11.1996 */
    case BAUD_115200    :Res=115200L;  break;  /*##20.11.1996 */
    case BAUD_128K  :Res=128000;    break;
    case BAUD_USER  :Res=999999;    break;
  }
  return (Res);
}


void COMMAPI ComSetState (BYTE Com,
			   LONGINT NewBaud,
			   BYTE  NewDataBits,
			   BYTE  NewStopBits,
				CHAR  NewParity,
			   BYTE  NewFlow)
{
DCB cb;
WORD linlen;

  if (!_ComValid(Com)) return;

  linlen=(WORD)ComBufSize(Com,DIR_INC);

  GetDCB(Com, &cb);

  cb.BaudRate=NewBaud;
  cb.fParity=TRUE;
  cb.fBinary=TRUE;
  cb.fErrorChar=FALSE; /*##27.3.1996*/
  cb.fNull=FALSE; /*##27.3.1996*/
  cb.ByteSize=NewDataBits;
  cb.EvtChar='\0'; /*##3.1.1997*/

  if (linlen)   /*##27.3.1996*/
  {     /* inlen - 0.9*inlen, this calculation is for NT only */
	cb.XoffLim=(WORD)(linlen-((9 * linlen)/10));
	    /* 0.1*inlen + inlen/100 * inlen/1000 */
    cb.XonLim =(WORD)((linlen/10) + ((linlen/100) * (linlen/1000)));
  }
  cb.XonChar=XON;
  cb.XoffChar=XOFF;
  cb.fTXContinueOnXoff=FALSE;
  cb.fDsrSensitivity=FALSE;

  cb.fAbortOnError=FALSE;  /*## 3.1.1997*/

  switch (NewParity)
  {
		 case 'E': cb.Parity=EVENPARITY; break;
		 case 'O': cb.Parity=ODDPARITY; break;
		 case 'S': cb.Parity=SPACEPARITY; break;
		 case 'M': cb.Parity=MARKPARITY; break;
		 default: cb.Parity=NOPARITY;  break; /* DEFAULT NO PARITY */
  }

  switch (NewStopBits)    /*##5.12.1996 */
 {
		 case 2: if (NewDataBits==5)
		           cb.StopBits=ONE5STOPBITS;  /* 1.5 StopBits */
		         else
		           cb.StopBits=TWOSTOPBITS;  /* 2 StopBits    */
			     break;
		 default:cb.StopBits=ONESTOPBIT;    /* 1 StopBit   */
			     break;
 }

 SetCommState(CHANNEL(Com).hCom, &cb);

 ComEnableFIFO(Com,THRESHOLD08);     /* Enable FIFO */
 ComSetFlow(Com,NewFlow);   /* Set flow */
 ComDTROn(Com);   /* Set output signals */
 ComRTSOn(Com);
 ComLineState(Com);    /* Clear bits */
 ComModemState(Com);   /* Clear bits */
 if (ComBufCount(Com,DIR_OUT)>0) ComEnableTX(Com);

 SetCommMask(CHANNEL(Com).hCom, _EV_ALL); /*##27.3.1996*/

 StartCommThread(Com);
}

void H_ComInit (BYTE Com, HANDLE hCom)
{
HANDLE hc;
CHAR COMNr[4];
CHAR COMName[15];  /*##16.1.1996*/
DWORD RxBufferSize;
DWORD TxBufferSize;
COMMTIMEOUTS ct;  

_InitCritSections();	/*##3.1.1997*/

  EnterCriticalSection(&GlobalCriticalSection);

  SCResult(Com)=scOk;
  if (PortsInUse==0) _InitAll();

  if (CHANNEL(Com).UserCount)   /* if port in use */
  {
     CHANNEL(Com).UserCount++;   /* increment use counter */
     LeaveCriticalSection(&GlobalCriticalSection); /*##12.11.1995*/
     return;
  }

  if (Com>COM_9)    /*##16.1.1996*/
    strcpy(COMName,"\\\\.\\COM");	 /* MS Bug */
  else
    strcpy(COMName,"COM");

  itoa((int)Com+1,COMNr,10);
  strcat(COMName,COMNr);

  if (hCom==INVALID_HANDLE_VALUE)
    hc=CreateFile(COMName,
		(GENERIC_READ|GENERIC_WRITE),
		 0, /* exclusive access */
		 NULL,  /* no securities */
		 OPEN_EXISTING,
		 FILE_ATTRIBUTE_NORMAL |  /*##19.11.96 added FILE_ATTRIBUTE_NORMAL */
#if defined (__USE_OVERLAPPING__)
       FILE_FLAG_OVERLAPPED,
#else
       FILE_FLAG_WRITE_THROUGH,
#endif
		 NULL);
  else         /*##22.11.1995*/
    hc=hCom;   /* use hCom */


  if (hc==INVALID_HANDLE_VALUE)
	 SCResult(Com)=scInvPort;
  else
	 {
#if defined(_DEBUG_)
  _DebugCom=Com;
#endif
  		InitializeCriticalSection(&CriticalPortSection[Com]);
		CHANNEL(Com).hCom=hc;
		CHANNEL(Com).UserCount++;  /* increment use counter */
        ComSaveState(Com);   /* save channel state */

        ClearReadCacheBuffer(Com);  /*##8.11.1995*/
		PortsInUse++;

		CHANNEL(Com)._DTR=SIGNAL_DTR;   /* DTR=On */
		CHANNEL(Com)._RTS=SIGNAL_RTS;   /* RTS=On */

		/* set RX buffer and TX buffer */
		/* first call after CreateFile */

/*      RxBufferSize=min(BUFFERSIZE_RX,_GetMaxBufferSize(Com,DIR_INC));
		TxBufferSize=min(BUFFERSIZE_TX,_GetMaxBufferSize(Com,DIR_OUT));
##15.11.1995*/
        RxBufferSize=_GetMaxBufferSize(Com,DIR_INC);  /*##15.11.1995*/
		TxBufferSize=_GetMaxBufferSize(Com,DIR_OUT);  /*##15.11.1995*/
		if (RxBufferSize==0L) RxBufferSize=BUFFERSIZE_RX;
		if (TxBufferSize==0L) TxBufferSize=BUFFERSIZE_TX;
		SetupComm(hc, RxBufferSize, TxBufferSize);

#if defined (__USE_OVERLAPPING__)
		ct.ReadIntervalTimeout=MAXDWORD; /*MAXDWORD=0xFFFFFFFF*/ /* 1 */
		ct.ReadTotalTimeoutMultiplier=0L;
		ct.ReadTotalTimeoutConstant=0L;

		ct.WriteTotalTimeoutMultiplier=0L;
		ct.WriteTotalTimeoutConstant=0L;

        memset(&SC_R_OS[Com],0,sizeof(OVERLAPPED)) ;
        memset(&SC_W_OS[Com],0,sizeof(OVERLAPPED)) ;

   /* create I/O event */
     SC_R_OS[Com].hEvent = CreateEvent(NULL,   /* no security         */
			     FALSE,  /* auto reset req  */
			     FALSE,  /* initial event reset */
			     NULL);  /* no name             */
     SC_W_OS[Com].hEvent = CreateEvent(NULL,   /* no security         */
			     FALSE,  /* auto reset req  */
			     FALSE,  /* initial event reset */
			     NULL);  /* no name             */

#else
		  /* set invinite timeout */
		ct.ReadIntervalTimeout=INFINITE /*0xFFFFFFFF*/;
		ct.ReadTotalTimeoutMultiplier=0L;
		ct.ReadTotalTimeoutConstant=0L;

		ct.WriteTotalTimeoutMultiplier=1L;
		ct.WriteTotalTimeoutConstant=1L;
#endif
		SetCommTimeouts(hc,&ct);

		ComBufClear(Com,DIR_INC);  /* clear buffers */
		ComBufClear(Com,DIR_OUT);
	    /* clear errors */
        ComLineState(Com);
        ComModemState(Com);
	 }

 LeaveCriticalSection(&GlobalCriticalSection); /*##12.11.1995*/
}

void COMMAPI ComInit (BYTE Com)
{
   H_ComInit(Com,INVALID_HANDLE_VALUE);
}
void COMMAPI ComInithCom (BYTE Com, HANDLE hCom)    /*##22.11.1995*/
{
   H_ComInit(Com, hCom);
}

void COMMAPI ComReset(BYTE Com)
{
  if (!_ComValid(Com)) return;

  EnterCriticalSection(&GlobalCriticalSection);  /*##12.11.1995*/

#if defined(__USE_OVERLAPPING__)
/*   SetEvent(SC_R_OS[Com].hEvent);	  signal event and stop WaitSingleObject */
/*   SetEvent(SC_W_OS[Com].hEvent);	  signal event and stop WaitSingleObject */

     CloseHandle(SC_R_OS[Com].hEvent);
     CloseHandle(SC_W_OS[Com].hEvent);
#endif 
  
	 if (CHANNEL(Com).UserCount>0) CHANNEL(Com).UserCount--;
	 if (CHANNEL(Com).UserCount==0)  /* Last user for this port */
	 {
        StopCommThread(Com);

		ComBufClear(Com,DIR_INC);
		ComBufClear(Com,DIR_OUT);

		ComRestoreState(Com,(PSAVESTATE)&CHANNEL(Com).SaveState);
		CloseHandle(CHANNEL(Com).hCom);

		_InitCom (Com);
  		DeleteCriticalSection(&CriticalPortSection[Com]);
		if (PortsInUse>0) PortsInUse--;
	 }

  LeaveCriticalSection(&GlobalCriticalSection);  /*##12.11.1995*/

  if (PortsInUse==0)  /*##12.11.1995*/
  {
	  _DeleteCritSections();	/*##3.1.1997*/
  }
}

WORD COMMAPI ComGetEventEx(BYTE Com)
{
WORD Event=0;
DWORD LastEvent;

  if (!_ComValid(Com)) return(0);

  LastEvent = _Crit_GetSetLastEvent(Com,0L);

  if (LastEvent&EV_TXEMPTY) Event|=evLAST_CHAR_SENT;
  if (LastEvent&EV_RXCHAR) Event|=evCHAR_RECEIVED;
  if (LastEvent&EV_BREAK) Event|=evBREAK_DETECTED;
  if (LastEvent&EV_ERR) Event|=evERROR_OCCURED;

  if (LastEvent&EV_CTS) Event|=evCTS_CHANGED;
  if (LastEvent&EV_DSR) Event|=evDSR_CHANGED;
  if (LastEvent&EV_RLSD) Event|=evDCD_CHANGED;
  if (LastEvent&EV_RING) Event|=evRI_CHANGED;

  return(Event);
}
BYTE COMMAPI ComGetEvent(BYTE Com)
{
BYTE Event=0;
DWORD LastEvent;

  if (!_ComValid(Com)) return(0xFF);

  LastEvent = _Crit_GetSetLastEvent(Com,0L);

  if (LastEvent&EV_TXEMPTY) Event|=evTHREMPTY;
  if (LastEvent&EV_RXCHAR) Event|=evRXRDY;
  if (LastEvent&(EV_BREAK | EV_ERR)) Event|=evEXCEPTION;
  if (LastEvent&(EV_CTS | EV_DSR | EV_RLSD | EV_RING)) Event|=evCHG_MODEM_STATUS;

  return(Event);
}

BYTE COMMAPI ComGetIRQVec(BYTE IRQ)
{
	if (IRQ<8)
	  return((BYTE)(IRQ+0x08));
	else
	  return((BYTE)(IRQ+0x68));
}

WORD COMMAPI ComGetXONXOFFChars(BYTE Com)
{
DCB cb;
  if (!_ComValid(Com)) return(0);
  GetDCB(Com,&cb);
  return((WORD)MAKEWORD(cb.XonChar,cb.XoffChar));
}

void COMMAPI ComSetXONXOFFChars  (BYTE Com, CHAR NewXONChar, CHAR NewXOFFChar )
{
DCB cb;
  if (!_ComValid(Com)) return;
  GetDCB(Com,&cb);
  cb.XonChar=NewXONChar;
  cb.XoffChar=NewXOFFChar;
  SetCommState(CHANNEL(Com).hCom,&cb);
}


    /* ReadFile using overlapped mode */
DWORD _fastcall osReadFile(BYTE Com, LPCHAR lpBuffer, DWORD dwCount)
{DWORD dwRdResult;
 HANDLE hCom;
#if defined (__USE_OVERLAPPING__)
 OVERLAPPED *pOS;
#endif

   dwRdResult=0L;
   hCom=CHANNEL(Com).hCom;

#if defined (__USE_OVERLAPPING__)
   pOS=&SC_R_OS[Com];

   if (! ReadFile(hCom,lpBuffer,dwCount,&dwRdResult,pOS))
   {
       if (GetLastError()==ERROR_IO_PENDING)
       {
	     GetOverlappedResult(hCom, pOS, &dwRdResult, TRUE);
       }
   }


#else
   ReadFile (hCom, lpBuffer, dwCount, &dwRdResult, NULL);
#endif

#if defined(_DEBUG_)
 Trace("osReadFile");
 Trace2(dwRdResult);
#endif

  return (dwRdResult);
}


    /* WriteFile using overlapped mode */
DWORD _fastcall osWriteFile(BYTE Com, LPCHAR lpBuffer, DWORD dwCount)
{DWORD dwWrResult;
 HANDLE hCom;
#if defined (__USE_OVERLAPPING__)
 LPOVERLAPPED pOS;
#endif

   dwWrResult=0L;
   hCom=CHANNEL(Com).hCom;

#if defined (__USE_OVERLAPPING__)
   pOS=&SC_W_OS[Com];

   if (! WriteFile (hCom, lpBuffer, dwCount, &dwWrResult, pOS))
   {
       if (GetLastError()==ERROR_IO_PENDING)
       {
	     GetOverlappedResult(hCom, pOS, &dwWrResult, TRUE);
       }
   }


#else
   WriteFile (hCom, lpBuffer, dwCount, &dwWrResult, NULL);
#endif

#if defined(_DEBUG_)
 Trace("osWriteFile");
 Trace2(dwWrResult);
#endif

   return (dwWrResult);
}

VOID _fastcall FillReadCache (BYTE Com, PREADCACHE pReadCache) /*##22.12.1996*/
{
 DWORD BufCount;
  
    Trace("FillReadCache Begin");

    BufCount = ComBufCount(Com, DIR_INC);
    if (BufCount)
    {
	  pReadCache->RdPtr=0L;
	  pReadCache->Count=osReadFile(Com,
	                               (LPVOID)(pReadCache->Buffer),
	                               min(BufCount, READCACHEBUFSIZE));
    }
#if defined(_DEBUG_)
 Trace("FillReadCache End");
 Trace2(BufCount);
 Trace2(pReadCache->Count);
#endif
}

DWORD COMMAPI ComReadEx (BYTE Com, PCHAR RCh, DWORD Count)
{
PREADCACHE pReadCache;
DWORD Res=0L;
DWORD cRead=0L;

  pReadCache=(PREADCACHE)&ReadCache[Com]; /*##8.11.1995*/

  while (Count)
  {
	if (pReadCache->Count == 0L) FillReadCache (Com, pReadCache);
	
	if (pReadCache->Count) /* chars in read cache ? */
      {                    /* yes */
/*      EnterCriticalSection(&GlobalCriticalSection);*/  /*##12.11.1995*/
        cRead=min(Count, pReadCache->Count);
      	/*
	    {DWORD i;
		  for (i=0L; i<cRead; i++)  RCh[Res+i]=pReadCache->Buffer[pReadCache->RdPtr+i];
      	}
		*/
		memcpy (&(RCh[Res]), &(pReadCache->Buffer[pReadCache->RdPtr]), cRead);
		
	    pReadCache->Count -= cRead;
        pReadCache->RdPtr += cRead;
        Count -= cRead;
		Res += cRead;
/*      LeaveCriticalSection(&GlobalCriticalSection);*/  /*##12.11.1995*/
      }
	else 
	  return(Res);
  }

  return(Res);
}


BOOLEAN _fastcall _CheckRXQueue (BYTE Com, PCHAR RCh, BOOLEAN Remove) /*## 19.1.1996*/
{
PREADCACHE pReadCache;

  pReadCache=(PREADCACHE)&ReadCache[Com]; /*##8.11.1995*/

  if (pReadCache->Count==0L)  /* chars in read cache ? */
  {                           /* no, fill read cache   */
	 FillReadCache (Com, pReadCache);
	 if (pReadCache->Count==0L) return(FALSE);  /* No chars to read */
  }

  *RCh = pReadCache->Buffer[pReadCache->RdPtr];
  
  if (Remove==TRUE)	 /* remove the character  ? */
  {
/*    EnterCriticalSection(&GlobalCriticalSection); */ /*##12.11.1995*/
     pReadCache->RdPtr++;
     if (pReadCache->RdPtr >= READCACHEBUFSIZE)
       pReadCache->RdPtr=0L;
     pReadCache->Count--;
/*    LeaveCriticalSection(&GlobalCriticalSection); */ /*##12.11.1995*/
  }

  return(TRUE);
}

BOOLEAN _fastcall _ComRead (BYTE Com, PCHAR RCh) /*## 9.1.1996*/
{
  return(_CheckRXQueue(Com, RCh, TRUE));
}

BOOLEAN COMMAPI ComSpyChar (BYTE Com, PCHAR RCh) /*## 19.1.1996*/
{
  return(_CheckRXQueue(Com,RCh,FALSE));
}

BOOLEAN COMMAPI ComUngetChar (BYTE Com, char SCh)
{
PREADCACHE pReadCache;
BOOLEAN Res=FALSE;

  pReadCache=(PREADCACHE)&ReadCache[Com]; /*##8.11.1995*/

  EnterCriticalSection(&GlobalCriticalSection);  /*##12.11.1995*/

  if (pReadCache->Count==0L) /* chars in read cache ? */
    {                        /* no, fill read cache   */
       pReadCache->RdPtr=0L;
       pReadCache->Buffer[0]=SCh;
       pReadCache->Count=1;
       Res=TRUE;  /* No chars to read */
    }
  else
    {
     if (pReadCache->Count < READCACHEBUFSIZE)  /* buffer not full ? */
     {
       pReadCache->RdPtr--;
       pReadCache->Buffer[pReadCache->RdPtr]=SCh;
       pReadCache->Count++;
       Res=TRUE;
     }
  }

  LeaveCriticalSection(&GlobalCriticalSection);  /*##12.11.1995*/

  return(Res);
}

DWORD COMMAPI ComWriteEx (BYTE Com, CHAR *SCh, DWORD Count)
{
DWORD BufSize;  /*##25.9.1995*/

  BufSize=ComBufSize(Com,DIR_OUT);

  if (BufSize>0L)
    Count=min((BufSize-ComBufCount(Com, DIR_OUT)), Count);
  else
    Count=1;

  return osWriteFile(Com, SCh, Count);
}
BOOLEAN _fastcall _ComWrite (BYTE Com, CHAR SCh)
{
  if (ComBufFull(Com,DIR_OUT)) return(FALSE);
  return((BOOLEAN)osWriteFile(Com,&SCh,1L)==1);
}

BOOLEAN _fastcall _ComKickOut (BYTE Com, CHAR SCh)
{
  if (!_ComValid(Com)) return(FALSE);
  return((BOOLEAN)TransmitCommChar(CHANNEL(Com).hCom,SCh));
}

BOOLEAN COMMAPI ComKickOutXOFF (BYTE Com)
{
  return ComKickOut(Com, HIBYTE(ComGetXONXOFFChars(Com)));
}

BOOLEAN COMMAPI ComKickOutXON (BYTE Com)
{
  return ComKickOut(Com, LOBYTE(ComGetXONXOFFChars(Com)));
}

DWORD _fastcall _ComBufCount (BYTE Com, TBufDir BufDir)
{
DWORD ChInQueue=0L;
DWORD err;
COMSTAT cstat;
  if (!_ComValid(Com)) return(0L);
  if (ClearCommError(CHANNEL(Com).hCom,&err,&cstat))
  {
	_HandleCommError(Com, err, &cstat);

    if (BufDir==DIR_INC)
      ChInQueue=(DWORD)(cstat.cbInQue + ReadCache[Com].Count); /*##8.11.1995*/
    else
      if (BufDir==DIR_OUT) ChInQueue=cstat.cbOutQue;
  }
  return(ChInQueue);
}
   /*##15.11.1995,  NEW !!! */
VOID COMMAPI ComSetBufSize (BYTE Com, DWORD RxBufferSize, DWORD TxBufferSize)
{
  if (!_ComValid(Com)) return;

  SetupComm(CHANNEL(Com).hCom, RxBufferSize, TxBufferSize);
}

DWORD COMMAPI ComBufSize (BYTE Com, TBufDir BufDir)
{
DWORD QueueSize=0L;
COMMPROP cp;
  if (!_ComValid(Com)) return(0L);
  GetCommProperties(CHANNEL(Com).hCom, &cp);

  if (BufDir==DIR_INC)
    QueueSize=cp.dwCurrentRxQueue;
  else
    if (BufDir==DIR_OUT) QueueSize=cp.dwCurrentTxQueue;

  return(QueueSize);
}
BOOLEAN COMMAPI ComBufFull  (BYTE Com, TBufDir BufDir)
{
DWORD BufCount;
   if (!_ComValid(Com)) return(TRUE);
   BufCount=ComBufCount(Com,BufDir);
   return((BOOLEAN)(BufCount && (BufCount==ComBufSize(Com,BufDir))));
}
BOOLEAN COMMAPI ComBufEmpty (BYTE Com, TBufDir BufDir)
{
   if (!_ComValid(Com)) return(FALSE);
   return((BOOLEAN)(ComBufCount(Com,BufDir)==0L));
}

void COMMAPI ComBufClear (BYTE Com, TBufDir BufDir)
{
  if (!_ComValid(Com)) return;
  if (BufDir==DIR_INC)
    {
     BYTE FlowControl=CHANNEL(Com).FlowControl;

      PurgeComm(CHANNEL(Com).hCom, PURGE_RXCLEAR);
      ClearReadCacheBuffer(Com); /*##8.11.1995*/

      if ((FlowControl & SIGNAL_XON)==SIGNAL_XON)
		  if (!ComXONState(Com)) ComKickOutXON(Com);

		if ((FlowControl & SIGNAL_DSR)==SIGNAL_DSR) ComDTROn(Com);
		if ((FlowControl & SIGNAL_CTS)==SIGNAL_CTS) ComRTSOn(Com);

	    CHANNEL(Com).fXON=TRUE;
		if (ComBufCount(Com,DIR_OUT)>0) ComEnableTX(Com);
    }
  else
    if (BufDir==DIR_OUT) PurgeComm(CHANNEL(Com).hCom, PURGE_TXCLEAR);
}

BYTE COMMAPI ComModemState (BYTE Com)
{
DWORD MSMask=0L;
BYTE Status=0;

   if (!_ComValid(Com)) return(0);
   if (GetCommModemStatus(CHANNEL(Com).hCom, &MSMask))
   {
     Status=(BYTE)(MSMask | CHANNEL(Com).ModemDelta);
     CHANNEL(Com).ModemDelta=0;
   }
   return(Status);
}
BYTE COMMAPI ComLineState  (BYTE Com)
{BYTE Res;
   if (!_ComValid(Com)) return(0);
   Res=_Crit_GetSetErrorState(Com,0);
   return(Res);
}
BOOLEAN COMMAPI ComXONState (BYTE Com)
{
   if (!_ComValid(Com)) return(FALSE);
   _GetCommError(Com);
   return(CHANNEL(Com).fXON);
}
void COMMAPI ComSetXONState (BYTE Com, BOOLEAN State)
{
  if (!_ComValid(Com)) return;
  if (State) 
    {
     ComEnableTX(Com);
     EscapeCommFunction (CHANNEL(Com).hCom, SETXON);
    }
  else
    {
     SetCommBreak(CHANNEL(Com).hCom);
     EscapeCommFunction (CHANNEL(Com).hCom, SETXOFF);
    }
  CHANNEL(Com).fXON=State;
}
void COMMAPI ComDTROn (BYTE Com)
{
   if (!_ComValid(Com)) return;
      /* if the signal is unused in flow control */
   if ((CHANNEL(Com).FlowControl & SIGNAL_DSR)==0)
   {
     EscapeCommFunction(CHANNEL(Com).hCom,SETDTR);
     CHANNEL(Com)._DTR=SIGNAL_DTR;
   }
}
void COMMAPI ComDTROff (BYTE Com)
{
   if (!_ComValid(Com)) return;
      /* if the signal is unused in flow control */
   if ((CHANNEL(Com).FlowControl & SIGNAL_DSR)==0)
   {
     EscapeCommFunction(CHANNEL(Com).hCom,CLRDTR);
     CHANNEL(Com)._DTR=0;
   }
}
void COMMAPI ComRTSOn (BYTE Com)
{
   if (!_ComValid(Com)) return;
      /* if the signal is unused in flow control */
   if ((CHANNEL(Com).FlowControl & SIGNAL_CTS)==0)
   {
     EscapeCommFunction(CHANNEL(Com).hCom,SETRTS);
     CHANNEL(Com)._RTS=SIGNAL_RTS;
   }
}
void COMMAPI ComRTSOff (BYTE Com)
{
   if (!_ComValid(Com)) return;
      /* if the signal is unused in flow control */
   if ((CHANNEL(Com).FlowControl & SIGNAL_CTS)==0)
   {
     EscapeCommFunction(CHANNEL(Com).hCom,CLRRTS);
     CHANNEL(Com)._RTS=0;
   }
}

void COMMAPI ComBreakOn (BYTE Com)
{
  if (!_ComValid(Com)) return; 
  EscapeCommFunction(CHANNEL(Com).hCom,SETBREAK);
}
void COMMAPI ComBreakOff (BYTE Com)
{
  if (!_ComValid(Com)) return; 
  EscapeCommFunction(CHANNEL(Com).hCom,CLRBREAK);
}

BYTE COMMAPI ComGetMCReg (BYTE Com)
{
   if (!_ComValid(Com)) return(0xFF);
   return((BYTE)(CHANNEL(Com)._RTS | CHANNEL(Com)._DTR));
}
void COMMAPI ComPutMCReg (BYTE Com, BYTE Val)
{
  if (!_ComValid(Com)) return;

    /* only SIGNAL_RTS + SIGNAL_DTR valid */
  if (Val&SIGNAL_RTS)
    ComRTSOn(Com);
  else
    ComRTSOff(Com);

  if (Val&SIGNAL_DTR)
    ComDTROn(Com);
  else
    ComDTROff(Com);
}
BOOLEAN COMMAPI ComFIFOEnabled (BYTE Com)
{
  return(TRUE);
}
BOOLEAN COMMAPI ComEnableFIFO (BYTE Com, BYTE Threshold)
{
  return(TRUE);
}
BOOLEAN COMMAPI ComDisableFIFO (BYTE Com)
{
  return(TRUE);
}

BOOLEAN COMMAPI ComValid(BYTE Com)
{
  return (_ComValid(Com));
}

void _fastcall _StoreState(BYTE Com, PSAVESTATE SaveState)
{
  GetDCB(Com,&SaveState->ComInfo);
  GetCommMask (CHANNEL(Com).hCom,&SaveState->EventMask);/*##27.3.1996*/
  GetCommTimeouts(CHANNEL(Com).hCom,&SaveState->CommTimeouts); /*##27.3.1996*/
}

void _fastcall _RestoreState(BYTE Com, PSAVESTATE SaveState)
{
  SetCommState(CHANNEL(Com).hCom,&SaveState->ComInfo);
  SetCommMask (CHANNEL(Com).hCom,SaveState->EventMask); /*##27.3.1996*/
  SetCommTimeouts(CHANNEL(Com).hCom,&SaveState->CommTimeouts); /*##27.3.1996*/
}

void COMMAPI ComSaveState(BYTE Com)
{
  _StoreState(Com,(PSAVESTATE)&CHANNEL(Com).SaveState);
}

void COMMAPI ComSaveStateEx(BYTE Com, PSAVESTATE SaveState)
{
  _StoreState(Com,(PSAVESTATE)SaveState);
}

void COMMAPI ComRestoreState(BYTE Com, PSAVESTATE SaveState)
{
   _RestoreState(Com,(PSAVESTATE)SaveState);
}
void COMMAPI ComXCHGState(BYTE Com, PSAVESTATE CurState, PSAVESTATE NewState)
{
  _StoreState (Com, CurState);
  _RestoreState(Com, NewState);
}

HANDLE COMMAPI ComGethCom(BYTE Com)
{
  return(CHANNEL(Com).hCom);
}

TSYSTIME COMMAPI ComGetSysTime(void)
{
   return((TSYSTIME)clock());
}

TSYSTIME COMMAPI ComGetElapsedTime(TSYSTIME Start)
{
TSYSTIME curclock;
     curclock=ComGetSysTime();
     if (curclock>=Start)
       return((TSYSTIME)(curclock-Start));
     else
       return((TSYSTIME)((DWORD)(0xFFFFFFFF-Start)+curclock));
}

WORD COMMAPI __GetUserCount(BYTE Com)
{
  return(CHANNEL(Com).UserCount);
}

BYTE COMMAPI ComPeekByte (BYTE Com, BYTE Bofs)
{
  return(ActComData[Com].UserData[Bofs]);
}
WORD COMMAPI ComPeekWord (BYTE Com, BYTE Bofs)
{
WORD *Ptr=(WORD *)&ActComData[Com].UserData[Bofs];
   return((WORD)*Ptr);
}
DWORD COMMAPI ComPeekLong (BYTE Com, BYTE Bofs)
{
DWORD *Ptr=(DWORD *)&ActComData[Com].UserData[Bofs];
   return((DWORD)*Ptr);
}

void COMMAPI ComPokeByte (BYTE Com, BYTE Bofs, BYTE Data)
{
  ActComData[Com].UserData[Bofs]=Data;
}
void COMMAPI ComPokeWord (BYTE Com, BYTE Bofs, WORD Data)
{
WORD *Ptr=(WORD *)&ActComData[Com].UserData[Bofs];
    *Ptr=Data;
}
void COMMAPI ComPokeLong (BYTE Com, BYTE Bofs, DWORD Data)
{
DWORD *Ptr=(DWORD *)&ActComData[Com].UserData[Bofs];
    *Ptr=Data;
}

/*---------  RS485 functions -----------*/

void RS485EnableTX(BYTE Com)  /* RS485 enable TX */
{
  ComRTSOn(Com);
}

void RS485EnableRX(BYTE Com)  /* RS485 enable RX */
{
  ComRTSOff(Com);
}

BOOLEAN COMMAPI ComRS485 (BYTE Com, BYTE Action) /* RS485ENABLE, RS485DISABLE, RS485STATUS */
{
  if (Com<MAX_COM)
	 {
      if (Action==RS485ENABLE) CHANNEL(Com).Flags|=flRS485;
	   if (Action==RS485DISABLE) CHANNEL(Com).Flags&=(BYTE)(~flRS485);
      return((BOOLEAN)(CHANNEL(Com).Flags & flRS485));
	 }
  else
	 return(FALSE);
}

void COMMAPI ComEnableTX(BYTE Com)
{
  if (ComRS485(Com,RS485STATUS)) RS485EnableTX(Com);
   ClearCommBreak(CHANNEL(Com).hCom);
}

BOOLEAN COMMAPI ComWrite (BYTE Com, char SCh)
{                            /* enable TX */
  if (ComRS485(Com,RS485STATUS)) RS485EnableTX(Com);
  return(_ComWrite(Com,SCh));
}

BOOLEAN COMMAPI ComKickOut (BYTE Com, char SCh)
{                           /* enable TX */
  if (ComRS485(Com,RS485STATUS)) RS485EnableTX(Com);
  return _ComKickOut(Com,SCh);
}

BOOLEAN COMMAPI ComRead (BYTE Com, LPCHAR RCh)
{
  if (ComRS485(Com,RS485STATUS))
  {    /* enable RX */
	 if (ComBufEmpty(Com,DIR_OUT)) RS485EnableRX(Com);
  }
  return(_ComRead(Com, RCh));
}

DWORD COMMAPI ComBufCount(BYTE Com, TBufDir BufDir)
{
DWORD Res;

  Res=_ComBufCount(Com,BufDir);

  if (ComRS485(Com,RS485STATUS) && (BufDir==DIR_INC))
  {
	  /* enable RX */
	 if (/*23.5.1996(Res==0) && */ComBufEmpty(Com,DIR_OUT)) RS485EnableRX(Com);
  }
  return(Res);
}

/*-------------------------------------------------*/

