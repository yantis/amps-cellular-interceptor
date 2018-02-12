/***********************************************/
/*              DataLink 3.022                 */
/*                                             */
/* Windows NT, OS/2                            */
/*                                             */
/* Copyright (c) 1991,96 by ADONTEC Ltd.       */
/***********************************************/

#if defined(_OS_NT_)
#define STRICT
#include <windows.h>
#endif

#if defined(_OS_OS2_)
#define INCL_BASE
#define INCL_DOSEXCEPTIONS
#include <os2.h>
#endif

#include "supercom.h"

#include "..\smrtheap.h"


#if defined(_OS_NT_)
extern CRITICAL_SECTION GlobalCriticalSection;  /*##12.11.1995*/
extern void _fastcall GetDCB(BYTE Com, LPDCB lpDCB); /*##20.11.1996 */
#endif

extern TComData ActComData[MAX_COM];

BOOLEAN yieldstate=TRUE;
BOOLEAN CtrlBreak =FALSE;
BOOLEAN _UsrBreak =FALSE;
#if defined (_OS_NT_)
DWORD SCTimeSlice =20L;  /*##6.2.1996, 5->20 ms*/
#endif
#if defined (_OS_OS2_)
ULONG SCTimeSlice =20L;  /*##6.2.1996, 5->20 ms*/
#endif

BOOLEAN AttentionPressed(void)
{
BOOLEAN st;
#if defined (_OS_NT_)
  EnterCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosEnterCritSec();
#endif

	st=(BOOLEAN)( _UsrBreak && CtrlBreak);

#if defined (_OS_NT_)
  LeaveCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosExitCritSec();
#endif
  return (st);
}

DWORD H_Delay (TSYSTIME WaitTime)  /* delay in ms,  ##15.11.1995 */
{
TSYSTIME Count;
TSYSTIME Start;

	Count=0L;
	Start=ComGetSysTime();
	while (Count<=WaitTime)  /*##12.11.1995 "<" --> "<="  */
	{
#if defined (_OS_NT_)
		Sleep((DWORD)SCTimeSlice);
#endif
#if defined (_OS_OS2_)
		DosSleep(SCTimeSlice);
#endif
		Count=ComGetElapsedTime(Start);
   }
   return(Count);
}

DWORD COMMAPI RS_Global_Yield(HWND hWnd)
{
  if (yieldstate)
	 return(H_Delay(SCTimeSlice));
  else
	 return(0);
}

BOOLEAN COMMAPI RS_GetYieldState(void)
{
  return (yieldstate);
}

VOID COMMAPI RS_SetYieldState(BOOLEAN State)
{
#if defined (_OS_NT_)
  EnterCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosEnterCritSec();
#endif

	yieldstate=State;

#if defined (_OS_NT_)
  LeaveCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosExitCritSec();
#endif
}

BOOLEAN COMMAPI RS_UserBreak(void)
{
BOOLEAN st;
#if defined (_OS_NT_)
  EnterCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosEnterCritSec();
#endif

	st = CtrlBreak;
	CtrlBreak=FALSE;

#if defined (_OS_NT_)
  LeaveCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosExitCritSec();
#endif
  return(st);
}

void COMMAPI RS_Delay (DWORD WaitTime)  /* delay in ms, ##15.11.1995 */
{
  H_Delay(WaitTime);
}

void COMMAPI RS_TXOnLine(BYTE Com, char SCh)
{
BOOLEAN OK;

   OK=FALSE;

   do
      {
       if (ComWrite(Com,SCh))
	 OK=TRUE;
       else
	 RS_Global_Yield(0);    /* give up Timeslice to System */
      }
	while (!( OK ));
}

void COMMAPI RS_RXOnLine(BYTE Com, char *RCh)
{
BOOLEAN OK;

   OK=FALSE;

   do
      {
       if (ComRead(Com,&(*RCh)))
	 OK=TRUE;
       else
	 RS_Global_Yield(0);    /* give up Timeslice to System */
      }
   while (!( OK));
}

BOOLEAN COMMAPI RS_TXInTime(BYTE Com, char SCh, TSYSTIME WaitTime)
{
TSYSTIME Start;
BOOLEAN TimeOut;
BOOLEAN OK;

   Start = ComGetSysTime();
   OK    = FALSE;

   do
     {
       if (ComWrite(Com,SCh))
	 OK=TRUE;
       else
	 RS_Global_Yield(0);    /* give up Timeslice to System */

       TimeOut=(BOOLEAN)(ComGetElapsedTime(Start)>WaitTime);
     }
   while (!(OK || TimeOut));

   return(OK);
}

BOOLEAN COMMAPI RS_RXInTime(BYTE Com, LPCHAR RCh, TSYSTIME WaitTime)
{
TSYSTIME Start;
BOOLEAN TimeOut;
BOOLEAN OK;

   Start  = ComGetSysTime();
   OK     = FALSE;

   do
    {
       if (ComRead(Com,&(*RCh)))
	 OK=TRUE;
       else
	 RS_Global_Yield(0);    /* give up Timeslice to System */

       TimeOut=(BOOLEAN)(ComGetElapsedTime(Start)>WaitTime);
    }
   while (!(OK || TimeOut));

   return(OK);
}

BYTE COMMAPI RS_ErrorState(BYTE Com)
{
BYTE es;
#if defined (_OS_NT_)
  EnterCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosEnterCritSec();
#endif
           /* store error bits in es=ErrorState */
	es=(BYTE)(ComLineState(Com) & ~(RX_READY | TX_READY));  /* clear bits */
		    /* clear error bits bevor return to caller */
	ActComData[Com].ErrorState&=~(OVERRUN_ERROR | PARITY_ERROR | FRAMING_ERROR | BREAK_DETECT);

#if defined (_OS_NT_)
  LeaveCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosExitCritSec();
#endif
  return(es);
}

BYTE COMMAPI RS_ModemState(BYTE Com)
{
  return(ComModemState(Com));
}

BYTE COMMAPI RS_LineState(BYTE Com)
{
BYTE st;
   st = (ComLineState(Com));

   if (!(ComBufFull(Com,DIR_OUT)))
      st =(BYTE) (st | TX_READY);

   if (!(ComBufEmpty(Com,DIR_INC)))
      st = (BYTE)(st | RX_READY);

    /* clear line status bits */
   ActComData[Com].ErrorState&=~(RX_READY | TX_READY);

   return(st);
}

void COMMAPI RS_Break(BYTE Com)
{
   ComBreakOn(Com);
   // RS_Delay(SEC0_3);        /* Delay > 250 ms   */
   Sleep(300);
   ComBreakOff(Com);
}

void COMMAPI RS_DisableUserBreak(void)
{
#if defined (_OS_NT_)
  EnterCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosEnterCritSec();
#endif

	 _UsrBreak=FALSE;
	 CtrlBreak=FALSE;

#if defined (_OS_NT_)
  LeaveCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosExitCritSec();
#endif
}

void COMMAPI RS_EnableUserBreak(void)
{
#if defined (_OS_NT_)
  EnterCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosEnterCritSec();
#endif

	 _UsrBreak=TRUE;
	 CtrlBreak=FALSE;

#if defined (_OS_NT_)
  LeaveCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosExitCritSec();
#endif
}

void COMMAPI RS_UserBreakPressed(void)
{
#if defined (_OS_NT_)
  EnterCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosEnterCritSec();
#endif

	CtrlBreak=TRUE;

#if defined (_OS_NT_)
  LeaveCriticalSection(&GlobalCriticalSection);
#endif
#if defined (_OS_OS2_)
  DosExitCritSec();
#endif
}

void COMMAPI RS_UserBreakOption(int Option)
{
  if (Option==brkNow) RS_UserBreakPressed();
}

BOOLEAN COMMAPI RS_GetXONState(BYTE Com)
{
  return (ComXONState(Com));
}

void COMMAPI RS_SetXONState(BYTE Com, BOOLEAN State)
{
  ComSetXONState(Com,State);
}

#if defined(_OS_NT_)
WORD COMMAPI RS_XOFFLimit(BYTE Com)
{
DCB cd;
  if (!ComValid(Com)) return(0);
  GetCommState(CHANNEL(Com).hCom, &cd);
  return(cd.XoffLim);
}
WORD COMMAPI RS_XONLimit(BYTE Com)
{
DCB cd;
  if (!ComValid(Com)) return(0);
  GetCommState(CHANNEL(Com).hCom, &cd);
  return(cd.XonLim);
}
void COMMAPI RS_SetXONLimit(BYTE Com, WORD XONLimit)
{
DCB cd;
  if (!ComValid(Com)) return;

  GetDCB(Com, &cd);   /*##20.11.1996 */

  cd.XonLim=XONLimit;
  SetCommState(CHANNEL(Com).hCom, &cd);
}
void COMMAPI RS_SetXOFFLimit(BYTE Com, WORD XOFFLimit)
{
DCB cd;
  if (!ComValid(Com)) return;

  GetDCB(Com, &cd);   /*##20.11.1996 */

  cd.XoffLim=XOFFLimit;
  SetCommState(CHANNEL(Com).hCom, &cd);
}
#endif

void COMMAPI RS_TXCtlChar(BYTE Com, char CtlChar, TSYSTIME minWait, TSYSTIME maxWait)
{
char Ch;
TSYSTIME Start;
	Start=ComGetSysTime();
	ComBufClear(Com,DIR_INC);
	while ((RS_RXInTime(Com,&Ch,minWait)) && (ComGetElapsedTime(Start)<maxWait))
	{
		ComBufClear(Com,DIR_INC);
	}
	ComWrite(Com,CtlChar);
}

