/***********************************************/
/*               MODEM 3.5                     */
/* COMMON                                      */
/* Copyright (c) 1991,96 by ADONTEC (R) Ltd.   */
/***********************************************/

#if defined(_OS_NT_)
#define STRICT
#include <windows.h>
#endif

#if defined (_OS_OS2_)
#define INCL_BASE
#include <os2.h>
#endif

#if defined (_OS_WIN_)
#include <windows.h>
#endif

#if defined (_OS_DOS_)
#define __SCINTERNAL__
#include <string.h>
#define farstrcpy _fstrcpy
#define farstrcat _fstrcat
#define farstrlen _fstrlen
#define farstrstr _fstrstr
#endif

#if defined (_OS_WIN_)
#define farstrcpy lstrcpy
#define farstrcat lstrcat
#define farstrlen lstrlen
#define farstrstr mystrstr
#endif

#if defined (_OS_NT_)||defined(_OS_OS2_)
#include <string.h>
#define farstrcpy strcpy
#define farstrcat strcat
#define farstrlen strlen
#define farstrstr strstr
#endif


#include <stdlib.h>

#include "supercom.h"


#if defined (_OS_WIN_)
#include <schook.h>
#endif

#include "..\smrtheap.h"


/* The function Connect can search trough modem result codes (messages).
	The AT command V1 should be used in the modem init string for the
	following result codes.
*/
#define MSTR_CONNECT   "CONNECT"    /* Connect OK */
#define MSTR_CARRIER   "CARRIER "   /* Connect OK */
#define MSTR_BUSY      "BUSY"       /* BUSY, no connection */
#define MSTR_NO        "NO "        /* no connection */
#define MSTR_NOCARRIER "NO CARRIER" /* no connection */


#if defined (_OS_DOS_)
BOOLEAN AttentionPressed(void);
#endif

#if defined (_OS_WIN_)
/* This function returns the address of the first occurrence of s2 in s1 */
LPSTRING mystrstr(LPSTRING Str1, LPSTRING Str2);
LPSTRING mystrstr(LPSTRING Str1, LPSTRING Str2)
{
LPSTRING s1, s2, ResStr;
BOOLEAN found;

	  if ((Str1==(LPSTRING)NULL) || (Str2==(LPSTRING)NULL))
       return((LPSTRING)NULL);

	  ResStr=(LPSTRING)NULL;
	  s1=(LPSTRING)Str1; s2=(LPSTRING)Str2; found=FALSE;

	  do
	  {  if (*s2==*s1)   /* check char */
		    {
		      if (!found)
            {
              ResStr=s1; /* first char found */
		        found=TRUE; /* ok cmp next char */
            }
            s1++;
            s2++;
		    }
        else
	       {
            s2=Str2; /* get first char again */
            if (found)
			     {
                ResStr=(LPSTRING)NULL;
			       found=FALSE;
              }
            else
              s1++;
		    }
     }
	  while (*s2 && *s1);

     if ((*s2==0) && found)
	    return(ResStr);
     else
       return((LPSTRING)NULL);
}
#endif

BOOLEAN COMMAPI RS_Carrier (BYTE Com)
{
BOOLEAN st;

	st=(BOOLEAN)((ComModemState(Com) & DATA_CARRIER_DETECT)!=0);
	return (st);

}


BOOLEAN COMMAPI RS_Ringing (BYTE Com)
{
BOOLEAN st;

	st=(BOOLEAN)((ComModemState(Com) & RING_INDICATOR)!=0);
	return (st);
}


void COMMAPI RS_HangUp (BYTE Com)
{
	ComDTROff (Com);
	// RS_Delay(SEC_1);
	Sleep(1000);

	ComDTROn(Com);
}


void COMMAPI RS_PickUp (BYTE Com)
{
	ComDTROn (Com);
}


void H_RS_Dial (BYTE Com, LPSTRING Number, BOOLEAN Enter);
void H_RS_Dial (BYTE Com, LPSTRING Number, BOOLEAN Enter)
{
WORD i;

	i=0;
	while ((Number[i]))
	{
		 RS_TXInTime (Com,Number[i],SEC0_5);
		 INC(i,(WORD)1);
	}

	if (Enter)
		RS_TXInTime(Com,CR,SEC0_5);
}

void COMMAPI RS_Dial (BYTE Com, LPSTRING Number)
{
	ComDTROn(Com);
        ComRTSOn(Com);
	H_RS_Dial (Com,Number,TRUE);
}

/* Connect can search trough modem result codes (messages).
	Therefore the AT command V1 should be used in the modem init string.
*/
BOOLEAN COMMAPI RS_Connect(BYTE Com, TSYSTIME ConnectDelay, BOOLEAN CheckModemResult, LPSTRING ModemResult, TINFOPROC InfoProc)
{
TSYSTIME Start;
WORD i;
char mres[MAXMODEMRESULT];
char Ch;
LPSTRING p;
BOOLEAN Done=FALSE;
BOOLEAN BreakProcess=FALSE;

	Start=ComGetSysTime();
	if (ModemResult)
	  p=ModemResult;
	else
	  p=(LPSTRING)&mres;

	i=0;
	p[i]='\0';

	while (!BreakProcess && !RS_Carrier(Com) && (ComGetElapsedTime(Start)<ConnectDelay) && (Done==FALSE))
	{
	  if ((InfoProc!=(TINFOPROC)NULL) && ((ComGetSysTime() % CONNECT_INFO_PERIOD)==0))
	  {
		 BreakProcess=(BOOLEAN)(InfoProc(Com,acCONNECT_INFO,(ConnectDelay-ComGetElapsedTime(Start)),(PFDATA)NULL)==infBREAK);
	  }
	  RS_Global_Yield(0);
	  if (CheckModemResult)
	  {
		 if (ComRead(Com,&Ch))
		 {
	 if (i==MAXMODEMRESULT-2) Done=TRUE;
	 p[i]=Ch; i++; p[i]=0;
	 if (Ch==CR)
	 {
		if (farstrstr(p,MSTR_CONNECT) ||  /* modem result code received */
			 farstrstr(p,MSTR_BUSY) ||
			 farstrstr(p,MSTR_NO) ||
			 farstrstr(p,MSTR_CARRIER)
			 ) Done=TRUE;
	 }
		 }
	  }
	}

	if (!RS_Carrier(Com) && CheckModemResult)
	{
	  Done=(BOOLEAN)((farstrstr(p,MSTR_CONNECT) || farstrstr(p,MSTR_CARRIER)) && !farstrstr(p,MSTR_NOCARRIER));
	}
	Done=(BOOLEAN)(RS_Carrier(Com)||Done);

	if (InfoProc!=(TINFOPROC)NULL)
	{
	  if (Done)
		 InfoProc(Com,acCONNECT_OK,(ConnectDelay-ComGetElapsedTime(Start)),(PFDATA)NULL);
	  else
		 InfoProc(Com,acCONNECT_FAILED,0L,(PFDATA)NULL);
	}
	return(Done);
}

void COMMAPI RSH_HangUp (BYTE Com)
{
	// RS_Delay (SEC_1);
	Sleep(1000);
	H_RS_Dial  (Com,"+++",FALSE);    /* Hayes Escape */
	// RS_Delay (SEC_1);
	Sleep(1000);
	RS_Dial  (Com,"ATH");    /* Hayes Hangup Command */
}


void COMMAPI RSH_PickUp (BYTE Com)
{
        ComDTROn(Com);
        ComRTSOn(Com);
	RS_Dial (Com,"ATA");
}

void COMMAPI RSH_ModemInit(BYTE Com, LPSTRING InitString)
{
	if (InitString==(LPSTRING)NULL)
	  RS_Dial (Com,HAYES_INIT_STRING);
	else
	  RS_Dial (Com,InitString);
}


