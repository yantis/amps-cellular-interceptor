/******************************************************/
/*                  SuperCom  3.53                    */
/*                                                    */
/* COMMON                                             */
/* Copyright (C) 1991,97 by ADONTEC(R) Ltd.           */
/******************************************************/

/******************************************************/
/* DigiCHANNEL PC/Xe or PC/Xi                         */
/* ---------------------------------------------------*/
/* Define _DIGI_PCXE_ if you are using these          */
/* intelligent boards.                                */
/*                                                    */
/* The intelligent version dosn't run in Protected    */
/* Mode under DOS.                                    */
/******************************************************/

/******************************************************/
/*      ---- 16 Bit DOS Extender only ----            */
/* ---------------------------------------------------*/
/* Phar Lap 286 DOS-Extender:                         */
/*  - define DOSX286 and rebuild the libraries        */
/*                                                    */
/* BORLAND DPMI16 (16Bit)                             */
/*  - do nothing                                      */
/*                                                    */
/* any other 16bit DOS-Extender:                      */
/*  - define __EXTDOS16__ and rebuild the libraries   */
/*    If this fails take a look at the lines marked   */
/*    with DOSX286 in COMINT.C and check your DOS-    */
/*    Extender documentation for equivalent functions */
/******************************************************/

#if !defined(__SUPERCOM__)
#define __SUPERCOM__

#if defined (_cplusplus_)||(__cplusplus)
extern "C" {
#endif

#include "..\smrtheap.h"


#if !defined(_OS_DOS_)&&!defined(_OS_WIN_)&&  \
    !defined(_OS_OS2_)&&!defined(_OS_NT_)
#error !!! define _OS_DOS_ or _OS_WIN_ or _OS_OS2_ or _OS_NT_ required !!!
#endif

#if (defined(_DIGI_PCXE_))&&(defined(_OS_OS2_)||defined(_OS_NT_))
#error !!! _DIGI_PCXE_ not needed for Windows NT  and OS/2
#endif

/* If a DOS extender is used but not Phar Lap */
/*
#define __EXTDOS16__
*/
/* In case of Phar Lap define DOSX286         */
#if defined (__EXTDOS16__) && defined(__SCINTERNAL__)    /* the file DOSEXT16.OBJ is required */
#define getvect PMI_GetPMIntVec
#define setvect PMI_SetPMIntVec
#endif

#if defined(_OS_WIN_)
#if defined(WINAPI)
#define COMMAPI WINAPI
#else
#define COMMAPI FAR PASCAL
#endif
#endif

#if defined (_OS_DOS_)
#define COMMAPI far pascal
#define FAR     far
#define NEAR    near
#define PASCAL  pascal
#define VOID    void
#endif

#if defined (_OS_OS2_)
#define COMMAPI EXPENTRY
#if (!defined(_MSC_VER)||defined(__IBMC__)||defined(__IBMCPP__)) && !defined(__FLAT__)
#define __FLAT__
#endif
#if !defined(_MSC_VER)||defined(__IBMC__)||defined(__IBMCPP__)
#if !defined(__WATCOMC__)
#define far
#define FAR
#endif
#endif
#endif

#if defined (_OS_NT_)||defined(WIN32)||defined(__WIN32__)
#if !defined(__FLAT__)
#define __FLAT__
#endif
#define COMMAPI WINAPI
/* These defines are used by ComInit.                  */
/* If you want to change the length of the buffers     */
/* change these constants and build the library again  */
#define BUFFERSIZE_RX   4096L   /* RX queue buffer len */
#define BUFFERSIZE_TX   1250L   /* TX queue buffer len */

//#define BUFFERSIZE_RX   4096L   /* RX queue buffer len */ 
//#define BUFFERSIZE_TX   64000L   /* TX queue buffer len */

#endif


/*
#if defined (__FLAT__)
#define COMMAPI pascal
#define FAR
#define NEAR
#define PASCAL  pascal
#define VOID    void
#endif
#endif
*/

/****************************************************/
/*                   ComInt                         */
/*                                                  */
/****************************************************/

#define COM_1      0
#define COM_2      1
#define COM_3      2
#define COM_4      3
#define COM_5      4
#define COM_6      5
#define COM_7      6
#define COM_8      7
#define COM_9      8
#define COM_10     9
#define COM_11    10
#define COM_12    11
#define COM_13    12
#define COM_14    13
#define COM_15    14
#define COM_16    15
#define COM_17    16
#define COM_18    17
#define COM_19    18
#define COM_20    19
#define COM_21    20
#define COM_22    21
#define COM_23    22
#define COM_24    23
#define COM_25    24
#define COM_26    25
#define COM_27    26
#define COM_28    27
#define COM_29    28
#define COM_30    29
#define COM_31    30
#define COM_32    31
#define COM_33    32
#define COM_34    33
#define COM_35    34
#define COM_36    35

#define MAX_ISR          14
#define MAX_COM          36
#define LAST_COM         COM_36

			  /* Up to 256 ports can be supported    */
			  /* by simply define more COM_ constanst */
/*    :
      :
#define COM_XX      YY
      :
#define COM_256    255

#define LAST_COM COM_XX
#define MAX_COM  XX
*/


#define LAST_RS232_COM   COM_10  /* RS232 Part 1 (Version 2 compatibel)*/
#define FIRST_RS232_COM2 COM_33  /* RS232 Part 2        */
#define LAST_RS232_COM2  COM_36  /* RS232 Part 2        */
#define LAST_MULTI_COM   COM_8   /* AST, ARNET          */
#define LAST_DIGI_COM    COM_32  /* DigiCHANNEL PC/X    */
#define LAST_STAR_COM    COM_32  /* StarGate or Hostess */


#define SOH        1
#define STX        2
#define BREAK      3
#define EOT        4
#define ACK        6
#define BEL        7
#define BS         8
#define TAB        9
#define LF        10
#define FF        12
#define CR        13
#define DC1       17
#define DC3       19
#define NAK       21
#define CAN       24
#define ESC       27
#define XON       17
#define XOFF      19



		 /* Function Result Codes, ComResult */
#define scOK           0    /* OK */
#define scINVPORT      1    /* Invalid Port, Port not connected */
#define scNOUSERBREAK  2    /* UserBreak-Future not installed   */
#define scNOPMINIT    10    /* cannot Init. Protected Mode Data */
#define scNOPMRESET   11    /* cannot Reset Protected Mode Data */
#define scINVBAUD     12    /* Baudrate not supported by driver */
#define scINVFUNC     13    /* Invalid function.                */

#define scOk           0    /* OK */
#define scInvPort      1    /* Invalid Port, Port not connected */
#define scNoUserBreak  2    /* UserBreak-Future not installed   */
#define scNoPMInit    10    /* cannot Init. Protected Mode Data */
#define scNoPMReset   11    /* cannot Reset Protected Mode Data */
#define scInvBaud     12    /* Baudrate not supported by driver */
#define scInvFunc     13    /* Invalid function.                */


#define scPCXWrongConfig  32   /* PC/X* board wrong configuration   */
#define scPCXNotFound     33   /* board is no PC/X* board           */
#define scPCXNoSign       34   /* init. of PC/X* board not possible */
#define scPCXCmdError     35   /* PC/X* cmd error                   */

#define FEP_CMDQUEUEFULL 128   /* FEP/OS command queue full         */



			/* ISR Mode  */
#define IER_ENABLE_RX        0x01
#define IER_ENABLE_TX        0x02
#define IER_ENABLE_ERROR     0x04
#define IER_ENABLE_MODEM     0x08
#define IER_ENABLE_ALL       0x0F    /* Default */
		  /* Com Drive Signals, Default */
#define SIGNAL_DTR            0x01
#define SIGNAL_RTS            0x02
#define SIGNAL_DTR_RTS        0x03   /* Default */
#define SIGNAL_OUT2           0x08
#define SIGNAL_DTR_OUT2       0x09
#define SIGNAL_RTS_OUT2       0x0A
#define SIGNAL_DTR_RTS_OUT2   0x0b
#define SIGNAL_LOOP           0x10
		  /* Flow Check Signals */
#define SIGNAL_NONE           0x00
#define SIGNAL_CTS            0x10
#define SIGNAL_DSR            0x20
#define SIGNAL_DSR_CTS        0x30
#define SIGNAL_XON            0x80
#define SIGNAL_DSR_CTS_XON    0xB0
		  /* Bits Of LineStatusReg */
#define RX_READY              0x01
#define OVERRUN_ERROR         0x02
#ifndef PARITY_ERROR
#define PARITY_ERROR          0x04
#define FRAMING_ERROR         0x08
#endif
#define BREAK_DETECT          0x10
#define TX_READY              0x20
		  /* Bits Of ModemStatusReg */
#define DELTA_CTS             0x01
#define DELTA_DSR             0x02
#define DELTA_RI              0x04
#define DELTA_DCD             0x08
#define CLEAR_TO_SEND         0x10
#define DATA_SET_READY        0x20
#define RING_INDICATOR        0x40
#define DATA_CARRIER_DETECT   0x80


#if !defined (brkChain)
#define brkChain    0
#define brkDiscard  1
#define brkNow      2
#endif


#define HAYES_INIT_STRING     "ATE0L1M0Q0V1&C1&D2&S1"  /* modem init string */
#define MAXMODEMRESULT        80  /* Length of string ModemResult in function Connect */


					  /* IRQ-Lines */
#define IRQ0                  0
#define IRQ1                  1
#define IRQ2                  2
#define IRQ3                  3
#define IRQ4                  4
#define IRQ5                  5
#define IRQ6                  6
#define IRQ7                  7
#define IRQ8                  8
#define IRQ9                  9
#define IRQ10                10
#define IRQ11                11
#define IRQ12                12
#define IRQ13                13
#define IRQ14                14
#define IRQ15                15

						 /* Vectors */
#define VEC1                  0x0C   /* Vector COM1/IRQ4 */
#define VEC2                  0x0B   /* Vector COM2/IRQ3 */

					  /* 16550 FIFO Threshold */
#define THRESHOLD01           0x00
#define THRESHOLD04           0x40
#define THRESHOLD08           0x80
#define THRESHOLD14           0xC0


					  /* Events reported from ComGetEvent  */
#define evCHG_MODEM_STATUS    0x01   /* normally Event 0 on UART */
#define evTHREMPTY            0x02
#define evRXRDY               0x04
#if defined(_OS_DOS_)||defined(_OS_WIN_)
#define evEXCEPTION           0x06
#else
#define evEXCEPTION           0x08
#endif
					  /* Events reported from ComGetEventEx */
#define evCHAR_RECEIVED       0x0001
#define evLAST_CHAR_SENT      0x0004
#define evCTS_CHANGED         0x0008
#define evDSR_CHANGED         0x0010
#define evDCD_CHANGED         0x0020
#define evBREAK_DETECTED      0x0040
#define evERROR_OCCURED       0x0080
#define evRI_DETECTED         0x0100
#define evRI_CHANGED          0x0200


#define DIR_INC               0      /* Buffer direction */
#define DIR_OUT               1

#if !defined(TRUE)
#define FALSE                 0      /* Boolean values */
#define TRUE                  1
#endif

#define IBUFSIZE              64
#define OBUFSIZE              64

#define RS232                  0
#define DigiPCX                1
#define Multi                  2
#define MultiSTARGATE          3
#define MultiARNET             4
#define DigiPCXi               5


			/* Reasons in AH within UserRXProc */
#define REASON_RXREADY   0x00
#define REASON_TXEMPTY   0x01
#define REASON_MSTATUS   0x02
#define REASON_ERROR     0x03
#define REASON_USER      0x20 /* user events schould be > REASON_USER */

		 /* ComRS485 Action */
#define RS485DISABLE  0
#define RS485ENABLE   1
#define RS485STATUS   2


#if defined(_OS_WIN_)
typedef DWORD FAR *LPDWORD;
typedef LPSTR LPSTRING;
typedef LPSTRING PathStr;
typedef char FAR *LPCHAR;
typedef short int SHORT;
typedef SHORT FAR *LPSHORT;
typedef FAR *POINTER;
typedef FAR *Pointer;
typedef char CHAR;
typedef WORD TSYSTIME;
#endif

#if defined (_OS_DOS_)
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef signed long LONG;
typedef unsigned long int DWORD;
typedef short int SHORT;
typedef char FAR *LPSTRING;
typedef char FAR *LPCHAR;
typedef SHORT FAR *LPSHORT;
typedef WORD FAR *LPWORD;
typedef DWORD FAR *LPDWORD;
typedef BYTE FAR *LPBYTE;
typedef LPSTRING PathStr;
typedef FAR *POINTER;
typedef FAR *Pointer;
typedef char CHAR;
typedef WORD TSYSTIME;
#endif


#if defined (_OS_OS2_)
typedef USHORT    WORD;
typedef ULONG     DWORD;
typedef PCHAR     LPSTRING;
typedef LPSTRING  PathStr;
typedef LPSTRING  LPCHAR;
typedef PVOID     POINTER;
typedef PVOID     Pointer;
typedef WORD      *PWORD;
typedef DWORD     *PDWORD;
typedef DWORD     TSYSTIME;
#if defined (__FLAT__)
typedef WORD      *LPWORD;
typedef DWORD     *LPDWORD;
typedef SHORT     *LPSHORT;
#else
typedef WORD  FAR *LPWORD;
typedef DWORD FAR *LPDWORD;
typedef SHORT FAR *LPSHORT;
#endif
#endif


#if defined(_OS_NT_)
typedef PVOID     POINTER;
typedef PVOID     Pointer;
typedef LPSTR     LPSTRING;
typedef LPSTRING  PathStr;
typedef LPSTRING  LPCHAR;
typedef SHORT     *PSHORT;
typedef WORD      *PWORD;
typedef DWORD     *PDWORD;
typedef SHORT     *LPSHORT;
typedef WORD      *LPWORD;
typedef DWORD     *LPDWORD;
typedef DWORD     TSYSTIME;
#endif

#if !defined(BOOLEAN)
typedef BYTE  BOOLEAN;
#endif

typedef long              LONGINT;
typedef LONGINT           LongInt;
typedef BYTE              TBufDir;
typedef BOOLEAN           Boolean;
typedef char              Char;


#define CHANNEL(Chn) ActComData[Chn]
#define SCResult(Chn) CHANNEL(Chn).Result


#if defined(__TURBOC__) && defined(__cplusplus)
#define __CPPARGS ...
#else
#define __CPPARGS
#endif

#if defined(_OS_DOS_)||defined(_OS_WIN_)
#if defined (__TURBOC__)
typedef void interrupt (far *TISR)(__CPPARGS);
#else
typedef void (interrupt far *TISR)(__CPPARGS);
#if !defined(__WATCOMC__)
#define asm _asm
#endif
#endif
#endif

#if defined(_OS_OS2_)||defined(_OS_NT_)
#if defined(__IBMC__)||defined(__IBMCPP__)
typedef void COMMAPI _TISR(__CPPARGS);
typedef _TISR *TISR;
#else
typedef void (COMMAPI *TISR)(__CPPARGS);
#endif
#endif

#if defined (__TURBOC__)
#define InPort(port) inportb(port)
#define ISRParam  (unsigned rbp, unsigned rdi, unsigned rsi,    \
			unsigned rds, unsigned res, unsigned rdx,       \
			unsigned rcx, unsigned rbx, unsigned rax,       \
			unsigned rip, unsigned rcs, unsigned flags)

#else
#if defined(__WATCOMC__)
#define enable _enable
#define disable _disable
#define ISRParam (union INTPACK r)
#else
#define disable() asm cli;
#define enable() asm sti;
#define ISRParam    (unsigned res, unsigned rds, unsigned rdi, \
			  unsigned rsi, unsigned rbp, unsigned rsp, \
			  unsigned rbx, unsigned rdx, unsigned rcx, \
			  unsigned rax, unsigned rip, unsigned rcs, \
			  unsigned flags)
#endif
#define InPort(port) inp(port)
#if !defined (_OS_WIN_)  /* DOS */
#define setvect _dos_setvect
#define getvect _dos_getvect
#endif
#endif


						/* special flags */
#define flRS485   0x01


typedef char TXBuffer[128];
typedef enum {NOANSWER,BEGINRX,ENDRX,CANCEL} HostAnswer;

#if defined (_OS_DOS_)||defined(_OS_WIN_)
typedef struct {
	 POINTER BufPtr;
	 WORD    BufLen;
	 WORD    BufCount;
	 WORD    WrPtr;
	 WORD    RdPtr;
} TBuffer;

typedef struct {
	 BYTE  BaudLo;
	 BYTE  BaudHi;
	 BYTE  IERegData;
	 BYTE  LCRegData;
	 BYTE  MCRegData;
	 BYTE  Spare;
} TSaveState;

typedef TSaveState FAR *PSAVESTATE;

typedef struct {
   WORD       XCom;
	WORD       IOReg;
	WORD       IEReg;
	WORD       IIReg;
	WORD       LCReg;
	WORD       MCReg;
	WORD       LSReg;
	WORD       MSReg;
	BYTE       ErrorState;
	BYTE       ModemState;


	TISR       Vec;
	TISR       LineErrProc;
	TBuffer    InBuffer;
	TBuffer    OutBuffer;
	BYTE       IRQ;
	BYTE       IRQVec;

	LONGINT    Baud;
	BYTE       DataBits;
	BYTE       StopBits;
	CHAR       Parity;
	BYTE       Mode;
	BOOLEAN    stXON;
	BOOLEAN    DoXON;
	WORD       XONLimit;
	WORD       XOFFLimit;
	BYTE       Queue;
	BYTE       FlowControl;

	CHAR       XONChar;
	CHAR       XOFFChar;
	BYTE       Event;
	BYTE       Flags;
	WORD       Result;

	TSaveState SaveState;

	TISR       UserMXProc;
	TISR       UserRXProc;
	TISR       UserTXProc;
	BYTE       UserData[16];
} TComData;


typedef TComData FAR *PComData;


typedef BYTE  TComIBuf[IBUFSIZE];
typedef BYTE  TComOBuf[OBUFSIZE];
#endif



#if defined(_OS_OS2_)||defined(_OS_NT_)
typedef struct { BYTE Lo;
		 BYTE Hi;
	       }stWORD;

typedef union  { stWORD h;
		 WORD x;
	       }TWORD;

typedef struct { WORD Lo;
		 WORD Hi;
	       }stDWORD;

typedef union  { stDWORD h;
		 ULONG x;
		}TDWORD;
#endif

#if defined(_OS_NT_)
typedef DCB TDCBINFO;

typedef struct {
		TDCBINFO     ComInfo;
		COMMTIMEOUTS CommTimeouts;
		DWORD        EventMask;
	       }TSaveState;

typedef TSaveState *PSAVESTATE;

typedef struct {
		HANDLE hCom;
		BOOLEAN fXON;
      BYTE _DTR;
      BYTE _RTS;
      BYTE FlowControl;
		WORD UserCount;
		WORD Result;
		BYTE ErrorState;
		BYTE ModemDelta;
      BYTE Res;
		BYTE Flags;
      TSaveState SaveState;
		BYTE UserData[16];
	       }TComData;

typedef TComData  *PComData;
#endif

#if defined(_OS_OS2_)
typedef struct {
		USHORT usWriteTimeout;
		USHORT usReadTimeout;
		BYTE   fbCtlHndShake;
		BYTE   fbFlowReplace;
		BYTE   fbTimeout;
		BYTE   bErrorReplacementChar;
		BYTE   bBreakReplacementChar;
		BYTE   bXONChar;
		BYTE   bXOFFChar;
	       }TDCBINFO;

typedef struct {
		BYTE bDataBits;
		BYTE bParity;
		BYTE bStopBits;
		BYTE fTransBreak;
	       }TLINECONTROL;

typedef struct {
		TDCBINFO     ComInfo;
		TLINECONTROL LineControl;
		ULONG        Baud;
	       }TSaveState;

typedef TSaveState *PSAVESTATE;

typedef struct {
		HFILE hCom;
		WORD UserCount;
		WORD Result;
		BYTE ErrorState;
		BYTE ModemDelta;
		BYTE Flags;
		BYTE Res;
		TSaveState SaveState;
		BYTE UserData[16];
	       }TComData;

typedef TComData  *PComData;
#endif


			/* supported protocols */
#define PROTOCOL_ASCII         0
#define PROTOCOL_XMODEM        1
#define PROTOCOL_XMODEMCRC     2
#define PROTOCOL_YMODEM        3
#define PROTOCOL_YMODEMBATCH   4
#define PROTOCOL_ZMODEM        5
#define PROTOCOL_USER         64    /* user protocols: PROTOCOL_USER + 1 etc. */


#define SC_MAXPATH 128

#ifndef _MAX_PATH
#define _MAX_PATH   80
#define _MAX_DRIVE   3
#define _MAX_DIR    66
#define _MAX_FNAME   9
#define _MAX_EXT     5
#endif
			 /* SuperCom error codes. Other codes are the DOS error codes. */
#define ERRTIMEOUT   (-9999)
#define ERRCANCELED  (-9998)
#define ERRBADLINE   (-9997)
#define ERRGOTCAN    (-9996)
#define ERRNOHEADER  (-9995)
#define ERRBADSTRUCT (-9994)

			 /* definitions used inside the  info function */
#define acTXing            0           /* transmitting package */
#define acTXING            0           /* transmitting package */
#define acRXing            1           /* receiving package */
#define acRXING            1           /* receiving package */
#define acUSERBREAK        2           /* Ctrl-Break */
#define acSYN              3           /* Synchronize */
#define acRetry            4           /* Retry */
#define acRETRY            4           /* Retry */
#define acFileName         5           /* receiving Filename */
#define acFILENAME         5           /* receiving Filename */
#define acRXFIN            6           /* succesfully rx */
#define acTXFIN            7           /* succesfully tx */
#define acFILEDATA         8           /* on tx request file data */
#define acCONNECT_INFO    32           /* info from RS_Connect */
#define acCONNECT_OK      33           /* from RS_Connect CONNECT succeed */
#define acCONNECT_FAILED  34           /* from RS_Connect CONNECT failed */

#define acUSER           128           /* user actions: acUSER + 1 etc. */


#define CONNECT_INFO_PERIOD  SEC_3     /* RS_Connect, info period */


#define WAITACK         SEC_10   /* Waiting period for Acknowledge */
#define WAITONECHAR     SEC_1    /* Waiting period for one char */
#define WAITHEADER      SEC_5    /* Waiting period for Header */
#define WAITSTARTHEADER SEC_60   /* Waiting period for ZRINIT Header */


				 /* Protocol Info */

#define MAXRETRYS    5           /* times */
#define NAKDELAY     1           /* 1 ms */



  /* return codes used in Info functions */
#define infBREAK              0  /* Break protocol */
#define infCONTINUE           1  /* continue protocol */
#define infZRECOVERY          2  /* start ZMODEM crash recovery */



typedef struct {
		WORD Year;  /* e.g. 1994 */
		WORD Month; /* 1..12   */
		WORD Day;   /* 1..31   */
		WORD Hour;  /* 0..23   */
		WORD Min;   /* 0..59   */
		WORD Sec;   /* 0..59   */
			 }TFDATE;

typedef struct {
		char FileName[SC_MAXPATH];   /* file name */
		WORD Protocol;               /* type of protocol */
		LONGINT FileSize;            /* file length */
		TFDATE FileDate;             /* file date */
			 }TFDATA;

#if defined(__FLAT__)
typedef TFDATA *PFDATA;
#else
typedef TFDATA FAR *PFDATA;
#endif



#ifndef MK_FP
#define MK_FP(seg,ofs) ((VOID FAR *)(((unsigned long)(seg)<<16)|(ofs)))
#endif


#if defined _MSC_VER
#if _MSC_VER>=700
#define inp(port) _inp(port)
#define outp(port,byte) _outp(port,byte)
#endif
#endif


#if !defined(HIWORD)
#define HIWORD(dw) ((WORD)((((DWORD)(dw))>>16)&0xFFFF))
#endif
#if !defined(LOWORD)
#define LOWORD(dw) ((WORD)(dw))
#endif
#if !defined(HIBYTE)
#define HIBYTE(w) ((BYTE)(((WORD)(w)>>8)&0xFF))
#endif
#if !defined(LOBYTE)
#define LOBYTE(w) ((BYTE)(w))
#endif

#if !defined(FIELDOFFSET)
#define FIELDOFFSET(structure,field) ((WORD)&(((structure *)0)->field))
#endif

#define WORD_LO(w) LOBYTE(w)
#define WORD_HI(w) HIBYTE(w)
#define INC(x,v) x+=v
#define MEMW(s,o) (*((LPWORD)MK_FP(s,o)))
#define MEMB(s,o) (*((LPBYTE)MK_FP(s,o)))

#ifndef MAKEWORD
#define MAKEWORD(Lo,Hi) (((WORD)(((BYTE)(Lo))|(((WORD)((BYTE)(Hi)))<<8))))
#endif
#ifndef MAKELONG
#define MAKELONG(Lo,Hi) (((LONG)(((WORD)(low))|(((DWORD)((WORD)(high)))<<16))))
#endif

#if !defined (max)
#define max(a,b) ((a>b)?(a):(b))
#endif
#if !defined(min)
#define min(a,b) ((a<b)?(a):(b))
#endif



#if defined (_DIGI_PCXE_)       /* if DigiCHANNEL PC/X* */
#include <digipcxe.h>
#endif




										/* Info function */
#if defined (_OS_DOS_)||defined(_OS_WIN_)
#if defined (__TURBOC__)
typedef int PASCAL (FAR *TINFOPROC)(BYTE Com, WORD Action, DWORD Data, PFDATA FileData);
#else
typedef int (PASCAL FAR *TINFOPROC)(BYTE Com, WORD Action, DWORD Data, PFDATA FileData);
#endif
#endif

#if defined(_OS_OS2_)||defined(_OS_NT_)
#if defined (__IBMC__)||defined(__IBMCPP__)
typedef int COMMAPI _TINFOPROC(BYTE Com, WORD Action, DWORD Data, PFDATA FileData);
typedef _TINFOPROC *TINFOPROC;
#else
typedef int (COMMAPI *TINFOPROC)(BYTE Com, WORD Action, DWORD Data, PFDATA FileData);
#endif
#endif


										/*##8.2.1996,  UserEventProc */
#if defined (_OS_DOS_)||defined(_OS_WIN_)
#if defined (__TURBOC__)
typedef DWORD PASCAL (FAR *PUSEREVENTPROC)(BYTE Com, DWORD SComEvent);
#else
typedef DWORD (PASCAL FAR *PUSEREVENTPROC)(BYTE Com, DWORD SComEvent);
#endif
#endif

#if defined(_OS_OS2_)||defined(_OS_NT_)
#if defined (__IBMC__)||defined(__IBMCPP__)
typedef DWORD COMMAPI _TUSEREVENTPROC(BYTE Com, DWORD SComEvent);
typedef _TUSEREVENTPROC *PUSEREVENTPROC;
#else
typedef DWORD (COMMAPI *PUSEREVENTPROC)(BYTE Com, DWORD SComEvent);
#endif
#endif


WORD COMMAPI ComGetVersion(void);

#if defined (_OS_DOS_)||defined(_OS_WIN_)
void COMMAPI ComDigiInit (BYTE Com, WORD Port, BYTE IRQNr, BYTE IntrVec);
void COMMAPI ComDigiSetStatusPort(WORD StatusPort);
WORD COMMAPI ComDigiGetStatusPort(void);

void COMMAPI ComMultiInit (BYTE Com, WORD Port, BYTE IRQNr, BYTE IntrVec);
void COMMAPI ComMultiSetStatusPort(WORD StatusPortLo, WORD StatusPortHi);
WORD COMMAPI ComMultiGetStatusPort(BYTE Com);

void COMMAPI ComStarInit (BYTE Com, WORD Port, BYTE IRQNr, BYTE IntrVec);
void COMMAPI ComStarSetStatusPort(WORD StatusPort1, WORD StatusPort2,
				  WORD StatusPort3, WORD StatusPort4);
WORD COMMAPI ComStarGetStatusPort(BYTE Com);

void COMMAPI ComHostessInit(BYTE Com, WORD Port, BYTE IRQNr, BYTE IntrVec);
void COMMAPI ComHostessSetStatusPort(WORD StatusPort1, WORD StatusPort2,
					  WORD StatusPort3, WORD StatusPort4);
WORD COMMAPI ComHostessGetStatusPort(BYTE Com);

void COMMAPI ComArnetInit (BYTE Com, WORD Port, BYTE IRQNr, BYTE IntrVec);
void COMMAPI ComArnetSetStatusPort(WORD StatusPort);
WORD COMMAPI ComArnetGetStatusPort(BYTE Com);
void COMMAPI ComInit (BYTE Com, WORD Port, BYTE IRQNr, BYTE IntrVec);

#else
void COMMAPI ComInit (BYTE Com);
#endif

#if defined (_OS_NT_)
void COMMAPI ComInithCom (BYTE Com, HANDLE hCom);
	/*##8.2.1996*/
PUSEREVENTPROC COMMAPI ComGetSetEventProc(BYTE Com, PUSEREVENTPROC EventProc);
#endif

WORD COMMAPI ComResult(BYTE Com);
void COMMAPI ComSaveState (BYTE Com);
void COMMAPI ComRestoreState(BYTE Com, PSAVESTATE SaveState);
void COMMAPI ComSaveStateEx(BYTE Com, PSAVESTATE SaveState);

void COMMAPI ComXCHGState(BYTE Com, PSAVESTATE CurState, PSAVESTATE NewState);

DWORD COMMAPI ComGetMaxBaudRate(BYTE Com);
DWORD COMMAPI ComGetBaudRate(BYTE Com);
void COMMAPI ComSetBaudRate(BYTE Com, LONGINT NewBaud);

#if defined(_OS_DOS_)||defined(_OS_WIN_)
LONGINT COMMAPI ComSetHighestBaudrate(LONGINT NewHighestBaudrate);
void COMMAPI ComOpen (BYTE Com, POINTER InBuf, WORD InLen,
			POINTER OutBuf, WORD OutLen);
void COMMAPI ComSetBuffer(BYTE Com, TBufDir BufDir, POINTER Buf, WORD BufLen);
void COMMAPI ComClose(BYTE Com);
TISR COMMAPI ComErrProc(BYTE Com);
void COMMAPI ComSetErrProc (BYTE Com, TISR ErrProc);
TISR COMMAPI ComSetUserMXProc (BYTE Com, TISR UserMXProc);
TISR COMMAPI ComSetUserRXProc (BYTE Com, TISR UserRXProc);
TISR COMMAPI ComSetUserTXProc (BYTE Com, TISR UserTXProc);
void COMMAPI ComCallUserRXProc(BYTE Com, BYTE Reason);
BOOLEAN COMMAPI ComSpyChar(BYTE Com, LPCHAR RCh);
BOOLEAN COMMAPI ComSpyCharEx(BYTE Com, LPCHAR RCh, TBufDir BufDir, WORD SpyPos);
BOOLEAN COMMAPI ComUngetChar (BYTE Com, char SCh);
char COMMAPI ComSwapChar(BYTE Com, TBufDir BufDir, WORD SpyPos, char NewCh);
WORD COMMAPI ComGetWritePos(BYTE Com, TBufDir BufDir);
WORD COMMAPI ComGetReadPos(BYTE Com, TBufDir BufDir);
void COMMAPI ComSetReadPos(BYTE Com, TBufDir BufDir, WORD NewReadPos);
void COMMAPI ComSetWritePos(BYTE Com, TBufDir BufDir, WORD NewWritePos);
void COMMAPI ComSetBufCount(BYTE Com, TBufDir BufDir, WORD NewCount);
void COMMAPI EnableIRQ(BYTE IRQ);
void COMMAPI DisableIRQ(BYTE IRQ);
void COMMAPI ComIRQProblem(BOOLEAN State);
BOOLEAN COMMAPI ComRotatePIC(BYTE IRQ);  /* IRQ = 0..7 */

WORD COMMAPI ComBufSize(BYTE Com, TBufDir BufDir);
WORD COMMAPI ComBufCount (BYTE Com, TBufDir BufDir);

#else     /* NT, 95, OS/2 */
DWORD COMMAPI ComBufSize(BYTE Com, TBufDir BufDir);
DWORD COMMAPI ComBufCount (BYTE Com, TBufDir BufDir);
#endif

TSYSTIME COMMAPI ComGetSysTime(void);
TSYSTIME COMMAPI ComGetElapsedTime(TSYSTIME Start);


void COMMAPI ComSetState (BYTE Com,
		  LongInt NewBaud,
		  BYTE    NewDataBits,
		  BYTE    NewStopBits,
		  Char    NewParity,
		  BYTE    NewFlow);

void COMMAPI ComReset(BYTE Com);
BYTE COMMAPI ComGetEvent(BYTE Com);
WORD COMMAPI ComGetEventEx(BYTE Com);
POINTER COMMAPI ComGetEventPtr(BYTE Com);
void COMMAPI ComSetXONXOFFChars (BYTE Com, char NewXONChar, char NewXOFFChar);
WORD COMMAPI ComGetXONXOFFChars(BYTE Com);
void COMMAPI ComSetXONState(BYTE Com, BYTE State);
BYTE COMMAPI ComPeekByte (BYTE Com, BYTE Bofs);
WORD COMMAPI ComPeekWord (BYTE Com, BYTE Bofs);
DWORD COMMAPI ComPeekLong (BYTE Com, BYTE Bofs);
void COMMAPI ComPokeByte (BYTE Com, BYTE Bofs, BYTE Data);
void COMMAPI ComPokeWord (BYTE Com, BYTE Bofs, WORD Data);
void COMMAPI ComPokeLong (BYTE Com, BYTE Bofs, DWORD Data);

BOOLEAN COMMAPI ComRead (BYTE Com, LPCHAR RCh);
BOOLEAN COMMAPI ComWrite (BYTE Com, char SCh);
BOOLEAN COMMAPI ComKickOut (BYTE Com, char SCh);
BOOLEAN COMMAPI ComKickOutXOFF (BYTE Com);
BOOLEAN COMMAPI ComKickOutXON (BYTE Com);

BOOLEAN COMMAPI ComBufFull  (BYTE Com, TBufDir BufDir);
BOOLEAN COMMAPI ComBufEmpty (BYTE Com, TBufDir BufDir);
void COMMAPI ComBufClear (BYTE Com, TBufDir BufDir);

#if defined(_OS_NT_) /*##7.1.1997*/
VOID COMMAPI ComSetBufSize (BYTE Com, DWORD RxBufferSize, DWORD TxBufferSize);
WORD COMMAPI ComThreadState(BYTE Com, WORD NewState);
 /*##19.1.1996*/
BOOLEAN COMMAPI ComSpyChar(BYTE Com, LPCHAR RCh);
BOOLEAN COMMAPI ComUngetChar (BYTE Com, char SCh);
#endif

void COMMAPI ComSetFlow (BYTE Com, BYTE Flow);
BYTE COMMAPI ComGetFlow (BYTE Com);
void COMMAPI ComEnableTX(BYTE Com);
BYTE COMMAPI ComModemState (BYTE Com);
BYTE COMMAPI ComLineState  (BYTE Com);
BOOLEAN COMMAPI ComXONState (BYTE Com);
void COMMAPI ComDTROn (BYTE Com);
void COMMAPI ComDTROff (BYTE Com);
void COMMAPI ComRTSOn (BYTE Com);
void COMMAPI ComRTSOff (BYTE Com);
void COMMAPI ComBreakOn (BYTE Com);
void COMMAPI ComBreakOff (BYTE Com);
BYTE COMMAPI ComGetMCReg (BYTE Com);
void COMMAPI ComPutMCReg (BYTE Com, BYTE Val);
void COMMAPI ComSetSCResult(BYTE Com, WORD Res);
BOOLEAN COMMAPI ComValid(BYTE Com);

BOOLEAN COMMAPI ComRS485 (BYTE Com, BYTE Action);

BOOLEAN COMMAPI ComFIFOEnabled (BYTE Com);
BOOLEAN COMMAPI ComEnableFIFO (BYTE Com, BYTE Threshold);
BOOLEAN COMMAPI ComDisableFIFO (BYTE Com);

BYTE COMMAPI ComGetIRQVec(BYTE IRQ);


#if defined (_OS_WIN_) || (defined(_OS_DOS_)&&!defined(_DIGI_PCXE_))
void COMMAPI OutPort(WORD Port, BYTE Val);
#endif
BOOLEAN COMMAPI Free_Seg (WORD Seg);
WORD  COMMAPI Get_Seg (WORD Seg);

#if defined (_OS_OS2_)||defined (_OS_NT_)
DWORD COMMAPI ComWriteEx (BYTE Com, PCHAR SCh, DWORD Count);
DWORD COMMAPI ComReadEx (BYTE Com, PCHAR RCh, DWORD Count);
#endif


/***********************************************/
/*              DataLink                       */
/***********************************************/

#if defined (_OS_DOS_)||defined(_OS_WIN_)
#define SEC0_05            1   /*  50 ms   */
#define SEC0_1             2   /* 100 ms   */
#define SEC0_3             6   /* 300 ms   */
#define SEC0_5             9   /* 500 ms   */
#define SEC_1             18   /*   1 sec  */
#define SEC_2             37   /*   2 sec  */
#define SEC_3             55   /*   3 sec  */
#define SEC_4             73   /*   4 sec  */
#define SEC_5             91   /*   5 sec  */
#define SEC_10           182   /*  10 sec  */
#define SEC_20           364   /*  20 sec  */
#define SEC_30           546   /*  30 sec  */
#define SEC_60          1092   /*   1 min  */
#endif

#if defined (_OS_WIN_)
LONG COMMAPI RS_Global_Yield(HWND hWnd);
#endif

#if defined (_OS_DOS_)
LONG COMMAPI RS_Global_Yield(WORD hWnd);
#if defined (__SCINTERNAL__)
BOOLEAN AttentionPressed(void);
#endif
#endif

#if defined (_OS_OS2_)||defined (_OS_NT_)
BOOLEAN AttentionPressed(void);

		  /* Time constants in ms  */
#define SEC0_05            50    /*  50 ms */
#define SEC0_1            100    /* 100 ms */
#define SEC0_2            200
#define SEC0_3            300
#define SEC0_4            400
#define SEC0_5            500    /* 1 sec */
#define SEC_1            1000
#define SEC_2            2000
#define SEC_3            3000
#define SEC_4            4000
#define SEC_5            5000
#define SEC_10          10000
#define SEC_20          20000
#define SEC_30          30000
#define SEC_40          40000
#define SEC_50          50000
#define SEC_60          60000    /* 1 min */
#endif

#if defined (_OS_NT_)
DWORD COMMAPI RS_Global_Yield(HWND hWnd); /*##15.11.1995*/
HANDLE COMMAPI ComGethCom(BYTE Com);
WORD COMMAPI __GetUserCount(BYTE Com);
#endif

#if defined (_OS_OS2_)
HFILE COMMAPI ComGethCom(BYTE Com);
DWORD COMMAPI RS_Global_Yield(HWND hWnd);  /*##15.11.1995*/
WORD COMMAPI __GetUserCount(BYTE Com);
#endif

BOOLEAN COMMAPI RS_GetYieldState(void);
void COMMAPI RS_SetYieldState(BOOLEAN State);

void COMMAPI RS_TXOnLine(BYTE Com, char SCh);
void COMMAPI RS_RXOnLine(BYTE Com, LPCHAR RCh);


BOOLEAN COMMAPI RS_TXInTime(BYTE Com, char SCh, TSYSTIME WaitTime);
BOOLEAN COMMAPI RS_RXInTime(BYTE Com, LPCHAR RCh, TSYSTIME WaitTime);

BYTE  COMMAPI  RS_ErrorState(BYTE Com);
BYTE  COMMAPI  RS_ModemState(BYTE Com);
BYTE  COMMAPI  RS_LineState(BYTE Com);

void  COMMAPI  RS_Break(BYTE Com);

WORD  COMMAPI  RS_XOFFLimit(BYTE Com);
WORD  COMMAPI  RS_XONLimit(BYTE Com);

BOOLEAN COMMAPI RS_UserBreak(void);
void  COMMAPI   RS_DisableUserBreak(void);
void  COMMAPI   RS_EnableUserBreak(void);
void  COMMAPI   RS_UserBreakOption(int Option);
void  COMMAPI   RS_UserBreakPressed(void);

void  COMMAPI   RS_Delay (TSYSTIME WaitTime);

void COMMAPI RS_SetXONLimit(BYTE Com, WORD XONLimit);
void COMMAPI RS_SetXOFFLimit(BYTE Com, WORD XOFFLimit);

BOOLEAN COMMAPI RS_GetXONState(BYTE Com);
void COMMAPI RS_SetXONState(BYTE Com, BOOLEAN State);

void COMMAPI RS_TXCtlChar(BYTE Com, char CtlChar, TSYSTIME minWait, TSYSTIME maxWait);



/***********************************************/
/*              Transport                      */
/***********************************************/

void COMMAPI RS_TXPacket(BYTE Com, LPCHAR Packet, WORD len, LPWORD  result);
void COMMAPI RS_RXPacket(BYTE Com, LPCHAR Packet, WORD len, LPWORD  result);

void COMMAPI RS_TXPInTime(BYTE Com, LPCHAR Packet, WORD len, TSYSTIME OneChDelay, LPWORD  result);
void COMMAPI RS_RXPInTime(BYTE Com, LPCHAR Packet, WORD len, TSYSTIME OneChDelay, LPWORD  result);

void COMMAPI RS_TXQPacket(BYTE Com, LPCHAR Packet, WORD len, LPWORD  result);
void COMMAPI RS_RXQPacket(BYTE Com, LPCHAR Packet, WORD len, LPWORD  result);

void COMMAPI Do_CRC(LPSHORT crc, char c);
BYTE COMMAPI RS_GetChkSum (LPCHAR Packet, WORD Len);
void COMMAPI RS_GetCRC_CCITT (LPCHAR Packet, WORD Len, LPSHORT CRC);

void COMMAPI Do_CRC32(LPDWORD crc, char c);
void COMMAPI RS_GetCRC_CCITT32 (LPCHAR Packet, WORD Len, LPDWORD crc);

LONGINT COMMAPI RS_GetTimeZone(void);
void COMMAPI RS_SetTimeZone(LONGINT TimeZone);
LONGINT COMMAPI RS_FileDateToUnix(WORD Year, WORD Month, WORD Day,
				WORD Hour, WORD Min, WORD Sec);
VOID COMMAPI RS_FileDateToDos (LONGINT DateInSec, LPWORD Year, LPWORD Month, LPWORD Day,
					 LPWORD Hour, LPWORD Min, LPWORD Sec);




/***********************************************/
/*               MODEM                         */
/***********************************************/


BOOLEAN COMMAPI RS_Carrier (BYTE Com);
BOOLEAN COMMAPI RS_Connect(BYTE Com, TSYSTIME ConnectDelay, BOOLEAN CheckModemResult, LPSTRING ModemResult, TINFOPROC InfoProc);
void COMMAPI RS_Dial    (BYTE Com, LPSTRING Number);
void COMMAPI RS_HangUp  (BYTE Com);
void COMMAPI RS_PickUp  (BYTE Com);
BOOLEAN COMMAPI RS_Ringing (BYTE Com);

void COMMAPI RSH_HangUp (BYTE Com);
void COMMAPI RSH_PickUp (BYTE Com);
void COMMAPI RSH_ModemInit(BYTE Com, LPSTRING InitString);


/****************************************************/
/*               Protocol 3.0                       */
/*                                                  */
/* Copyright (c) 1991,94 by ADONTEC (R) Ltd.        */
/****************************************************/


/***********************************************/
/*                   PROTCOM                   */
/***********************************************/

void COMMAPI RS_PackFileData(LPSTRING dest, PFDATA FData);
void COMMAPI RS_UnPackFileData(PFDATA FData, LPSTRING src);

void COMMAPI RS_WaitLineClear(BYTE Com, TBufDir BufDir, TSYSTIME Timedelay);

LPSTRING COMMAPI ValToOct(DWORD dw, LPSTRING OctStr);
DWORD COMMAPI OctToVal(LPSTRING OctStr);

/***********************************************/
/*                ASCII                        */
/***********************************************/

BOOLEAN COMMAPI RS_TXFile_ASCII(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);
BOOLEAN COMMAPI RS_RXFile_ASCII(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);

/***********************************************/
/*               XMODEM                        */
/***********************************************/

BOOLEAN COMMAPI RS_TXFile_XMODEM(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);
BOOLEAN COMMAPI RS_RXFile_XMODEM(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);

/***********************************************/
/*               XMODEMCRC                     */
/***********************************************/

BOOLEAN COMMAPI RS_TXFile_XMODEMCRC(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);
BOOLEAN COMMAPI RS_RXFile_XMODEMCRC(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);

/***********************************************/
/*               YMODEM                        */
/***********************************************/

BOOLEAN COMMAPI RS_TXFile_YMODEM(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);
BOOLEAN COMMAPI RS_RXFile_YMODEM(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);

/***********************************************/
/*              YMODEM  Batch                  */
/***********************************************/

BOOLEAN COMMAPI RS_TXFile_YMODEM_BATCH(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);
BOOLEAN COMMAPI RS_RXFile_YMODEM_BATCH(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);

/***********************************************/
/*               ZMODEM                        */
/***********************************************/

BOOLEAN COMMAPI RS_TXFile_ZMODEM(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);
BOOLEAN COMMAPI RS_RXFile_ZMODEM(BYTE Com, PathStr FileName, int FAR *ErrorCode, TINFOPROC InfoProc);



/****************************************************/
/*               Terminal  3.0                      */
/*                                                  */
/*                                                  */
/* Copyright (C) 1991,94 by ADONTEC(R) Ltd.         */
/****************************************************/

/* ANSI Terminal Emulation */

typedef struct {
					  SHORT X;
					  SHORT Y;
					}TSCRPOS;
typedef TSCRPOS FAR *PSCRPOS;


/* ANSI tERMINAL cOMMANDS send to remote process */
typedef enum {tcANSINONE, tcANSICURSORUP, tcANSICURSORDOWN, tcANSICURSORRIGHT,
	      tcANSICURSORLEFT, tcANSICURSORHOME, tcANSIGOTOXY,
	      tcANSICLEARSCREEN, tcANSICLEAREOL, tcANSIGETXY,
	      tcANSISAVEXY, tcANSIGOTOSAVEDXY} _TANSITERMCMD;

/* cOMMANDS translated by own call back function */
typedef enum {cfANSINONE, cfANSIGETXY, cfANSIGOTOXY, cfANSICHECKXY,
	      cfANSICLEARSCR, cfANSICLEAREOL, cfANSISETVIDEO,
			cfANSIUSER} _TANSIACTION;

typedef BYTE TANSITERMCMD;
typedef BYTE TANSIACTION;

#if defined (__TURBOC__)
typedef LONGINT COMMAPI (*PANSIPROC)(BYTE Com, TANSIACTION Action, WORD wParam, POINTER pParam);
#else
#if (defined(__IBMC__)||defined(__IBMCPP__))&&defined(_OS_OS2_)
typedef LONGINT COMMAPI _TANSIPROC(BYTE Com, TANSIACTION Action, WORD wParam, POINTER pParam);
typedef _TANSIPROC *PANSIPROC;
#else
typedef LONGINT (COMMAPI *PANSIPROC)(BYTE Com, TANSIACTION Action, WORD wParam, POINTER pParam);
#endif
#endif

	 /* ANSI internal scan state*/
#define scanESC     1
#define scanESC2    2
#define scanDATA    3


#define ANSISTRING 20

typedef struct {
	 /* Public data */
       PANSIPROC ANSIProc;

  /* Private data don't touch */
		 char              Msg[ANSISTRING];
		 BYTE              ScanState;
		 BOOLEAN           RXRemXY;    /* Received remote X,Y */
		 TSCRPOS           RemScrPos;  /* Remote screen position */
		 TSCRPOS           SaveScrPos; /* Remote screen position */
	   }TANSIDATA;
typedef TANSIDATA FAR *PANSIDATA;

BOOLEAN COMMAPI RS_ANSIInit(BYTE Com, PANSIDATA TermData);
BOOLEAN COMMAPI RS_ANSIRead(BYTE Com, LPCHAR RCh);
BOOLEAN COMMAPI RS_TranslateToANSI(TANSITERMCMD Cmd, PSCRPOS XYPos, LPSTRING ANSIString);
BOOLEAN COMMAPI RS_ANSIGetXY(BYTE Com, PSCRPOS ScrPos);



/* TTY Terminal Emulation */

/* TTY tERMINAL cOMMANDS send to remote process */
typedef enum {tcBEL, tcBS, tcTAB, tcLF, tcFF, tcCR} _TTTYTERMCMD;


/* cOMMANDS translated by own call back function */
typedef enum {cfTTYNONE, cfTTYGETXY, cfTTYGOTOXY, cfTTYCHECKXY,
				  cfTTYCLEARSCR} _TTTYACTION;
typedef BYTE TTTYTERMCMD;
typedef BYTE TTTYACTION;

#if defined (__TURBOC__)
typedef LONGINT COMMAPI (*PTTYPROC)(BYTE Com, TTTYACTION Action, WORD wParam, POINTER pParam);
#else
#if (defined(__IBMC__)||defined(__IBMCPP__))&&defined(_OS_OS2_)
typedef LONGINT COMMAPI _TTTYPROC(BYTE Com, TTTYACTION Action, WORD wParam, POINTER pParam);
typedef _TTTYPROC *PTTYPROC;
#else
typedef LONGINT (COMMAPI *PTTYPROC)(BYTE Com, TTTYACTION Action, WORD wParam, POINTER pParam);
#endif
#endif

#define TTYSTRING 10

typedef struct {
	 /* Public data */
       PTTYPROC TTYProc;
       BYTE TABLen;
       BYTE AddLF;

  /* Private data don't touch */
		 char              Msg[ANSISTRING];
	   }TTTYDATA;
typedef TTTYDATA FAR *PTTYDATA;

BOOLEAN COMMAPI RS_TTYInit(BYTE Com, PTTYDATA TermData);
BOOLEAN COMMAPI RS_TTYRead(BYTE Com, LPCHAR RCh);
BOOLEAN COMMAPI RS_TranslateToTTY(TTTYTERMCMD Cmd, LPSTRING TTYString);




/* VT52 Terminal Emulation */


/* VT52 tERMINAL cOMMANDS send to remote process */
typedef enum {tcVT52NONE, tcVT52CURSORUP, tcVT52CURSORDOWN, tcVT52CURSORRIGHT,
	      tcVT52CURSORLEFT, tcVT52CURSORHOME, tcVT52GOTOXY,
	      tcVT52CLEARSCR, tcVT52CLEAREOL, tcVT52CLEAREOS} _TVT52TERMCMD;


/* cOMMANDS translated by own call back function */
typedef enum {cfVT52NONE, cfVT52GETXY, cfVT52GOTOXY, cfVT52CHECKXY,
		   cfVT52CLEAREOS, cfVT52CLEAREOL, cfVT52USER} _TVT52ACTION;

typedef BYTE TVT52TERMCMD;
typedef BYTE TVT52ACTION;

#if defined (__TURBOC__)
typedef LONGINT COMMAPI (*PVT52PROC)(BYTE Com, TVT52ACTION Action, WORD wParam, POINTER pParam);
#else
#if (defined(__IBMC__)||defined(__IBMCPP__))&&defined(_OS_OS2_)
typedef LONGINT COMMAPI _TVT52PROC(BYTE Com, TVT52ACTION Action, WORD wParam, POINTER pParam);
typedef _TVT52PROC *PVT52PROC;
#else
typedef LONGINT (COMMAPI *PVT52PROC)(BYTE Com, TVT52ACTION Action, WORD wParam, POINTER pParam);
#endif
#endif

	 /* VT52 internal scan state*/
#define scanESC     1
#define scanDATA    3


#define VT52STRING 20

typedef struct {
	 /* Public data */
		 PVT52PROC VT52Proc;

  /* Private data don't touch */
		 char              Msg[VT52STRING];
		 BYTE              ScanState;
		}TVT52DATA;
typedef TVT52DATA FAR *PVT52DATA;

BOOLEAN COMMAPI RS_VT52Init(BYTE Com, PVT52DATA TermData);
BOOLEAN COMMAPI RS_VT52Read(BYTE Com, LPCHAR RCh);
BOOLEAN COMMAPI RS_TranslateToVT52(TVT52TERMCMD Cmd, PSCRPOS XYPos, LPSTRING VT52String);




#if defined (_OS_OS2_)
#undef COMMAPI
#if defined(__IBMC__)||defined(__IBMCPP__)
#define COMMAPI EXPENTRY _Export
#else
#define COMMAPI EXPENTRY _export
#endif
#endif

#if defined(_OS_NT_)
#undef COMMAPI
#define COMMAPI WINAPI 
#endif


#if defined (_cplusplus_)||defined(__cplusplus)
}
#endif

#endif /* __SUPERCOM__ */



