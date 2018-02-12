// CELLULARDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CELLULAR.h"
#include "CELLULARDlg.h"

#include <sys/types.h>
#include <sys/stat.h>

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

#include "supercom\supercom.h"

#include "area.h"
#include "phonelst.h"
#include "manufact.h"


#include "smrtheap.hpp"

#define OK 0
#define CLOSED 0
#define OPEN 1

// #define RECORD

///////////////////////////
int iCommPortOS456; 
int iCommPortDDI;
long int iMaxCallTime; 
bool bReverseChannel;


int iBufferCounter;
char dateis[15];
char timeis[15];
char d_timeis[12];

char chDDIdataBuffer[60]; 
char chPhoneNumber[15];
char chRegistration[2];
char chArea[38];
char chChannel[5];
char chGoFrequency[8];
char chAllRegistrations[4];
char chAllPaging[4];
char chRegistrationType[2];
char chAreaCode[4];
char chWhereErrorOccured[100];

int endnum;
int stanum;

bool bRegistration = false;
bool bDisconnect = false;
bool bPaging = false;
bool bReorder = false;
bool bLockOut = false;
bool bGoto    = false; 
bool bVoiceGoto = false;
bool bAllPaging = false;
bool bAllRegistration = false;

bool bNewTest;
bool bNewNumber;
bool bAmend;

char chChannelBand[2];

char chSCMstring[50];

//////////////  ISAM
dfAREA areabase;
dfPHONELST phonelst;
dfMANUFACT manufact;

////////////////////

bool bCancelCall;
char chSignalStrength[100];
time_t start_time;

int squelch;
bool bHoldCall;

int iCallCounter_0;
int iCallCounter_1;
int iCallCounter_2;
int iCallCounter_3;
int iCallCounter_4;
int iCallCounter_5;
int iCallCounter_6;
int iCallCounter_7;
int iCallCounter_8;
int iCallCounter_9;
int iCallCounter_total;
int iCallCounter_record;
int iCallCounter_fax;
int iCallCounter_modem;


int iWaveFileNumber;
UINT wDeviceID;

bool bMonitor;
bool bAquire;
bool bGoReverseVoice;
bool bGoForwardVoice;
bool bVoiceForward;

char chChannelType[50];
char chFrequency[50];

char chReverseChannel[50];
char chForwardChannel[50];

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCELLULARDlg dialog

CCELLULARDlg::CCELLULARDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCELLULARDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCELLULARDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCELLULARDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCELLULARDlg)
	DDX_Control(pDX, IDC_PROGRESS_SIGNAL, m_ProgressSignal);
	DDX_Control(pDX, IDC_BUTTON_CONTROL_TYPE, m_ButtonSwitchControlType);
	DDX_Control(pDX, IDC_BUTTON_SWITCHVOICE, m_ButtonSwitchVoice);
	DDX_Control(pDX, IDC_EDIT_CALLS, m_EditCallList);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL9, m_Text_CallsLevel9);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL8, m_Text_CallsLevel8);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL7, m_Text_CallsLevel7);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL6, m_Text_CallsLevel6);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL5, m_Text_CallsLevel5);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL4, m_Text_CallsLevel4);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL3, m_Text_CallsLevel3);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL2, m_Text_CallsLevel2);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL1, m_Text_CallsLevel1);
	DDX_Control(pDX, IDC_TEXT_CALLSLEVEL0, m_Text_CallsLevel0);
    DDX_Control(pDX, IDC_TEXT_TOTALCALL, m_Text_CallsTotal);
	DDX_Control(pDX, IDC_TEXT_CALLSRECORD, m_Text_CallsRecorded);
	DDX_Control(pDX, IDC_TEXT_CALLSMODEM, m_Text_CallsModem);
	DDX_Control(pDX, IDC_TEXT_CALLSFAX, m_Text_CallsFax);
	DDX_Control(pDX, IDC_EDIT_TOTALCALLS, m_Edit_TotalCalls);
	DDX_Control(pDX, IDC_EDIT_SUBJECT, m_EditSubject);
	DDX_Control(pDX, IDC_EDIT_SIGNALSTR, m_EditSignalStrength);
	DDX_Control(pDX, IDC_EDIT_SCM, m_EditSCM);
	DDX_Control(pDX, IDC_EDIT_RECORDCALL, m_EditRecordCall);
	DDX_Control(pDX, IDC_EDIT_PHONETYPE, m_EditPhoneType);
	DDX_Control(pDX, IDC_EDIT_PHONEMODEL, m_EditPhoneModel);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_EditPhone);
	DDX_Control(pDX, IDC_EDIT_OCCUPATION, m_EditOccupation);
	DDX_Control(pDX, IDC_EDIT_NAME2, m_EditName2);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EditName);
	DDX_Control(pDX, IDC_EDIT_MODEMCALL, m_EditModemCalls);
	DDX_Control(pDX, IDC_EDIT_LINKEDFILE, m_EditLinkedFile);
	DDX_Control(pDX, IDC_EDIT_LASTCALL, m_EditLastCall);
	DDX_Control(pDX, IDC_EDIT_LANGUAGE, m_EditLanguage);
	DDX_Control(pDX, IDC_EDIT_INFO3, m_EditInfo3);
	DDX_Control(pDX, IDC_EDIT_INFO2, m_EditInfo2);
	DDX_Control(pDX, IDC_EDIT_INFO1, m_EditInfo1);
	DDX_Control(pDX, IDC_EDIT_GENDER, m_EditGender);
	DDX_Control(pDX, IDC_EDIT_FAXCALLS, m_EditFaxCalls);
	DDX_Control(pDX, IDC_EDIT_ESN, m_EditESN);
	DDX_Control(pDX, IDC_EDIT_DTMF, m_EditDtmf);
	DDX_Control(pDX, IDC_EDIT_DIALEDNUMBER, m_EditDialedNumber);
	DDX_Control(pDX, IDC_EDIT_CHANNELAB, m_EditChannelAB);
	DDX_Control(pDX, IDC_EDIT_CALLTIME, m_EditCallTime);
	DDX_Control(pDX, IDC_EDIT_CALLSTART, m_EditStart);
	DDX_Control(pDX, IDC_EDIT_CALLEND, m_EditCallEnd);
	DDX_Control(pDX, IDC_EDIT_AREA, m_EditArea);
	DDX_Control(pDX, IDC_EDIT_ALERTLEVEL, m_EditAlertLevel);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_Edit_Status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCELLULARDlg, CDialog)
	//{{AFX_MSG_MAP(CCELLULARDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_ERRSPACE(IDC_EDIT_STATUS, OnErrspaceEditStatus)
	ON_BN_CLICKED(IDC_BUTTON_SKIPVOICECALL, OnButtonSkipvoicecall)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA, OnButtonSaveData)
	ON_BN_CLICKED(IDC_BUTTON_FIND_PHONE, OnButtonFindPhone)
	ON_BN_CLICKED(IDC_CHECK_HOLDCALL, OnCheckHoldcall)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_AQUIRE, OnCheckAquire)
	ON_BN_CLICKED(IDC_CHECK_MONITOR, OnCheckMonitor)
	ON_BN_CLICKED(IDC_BUTTON_SWITCHVOICE, OnButtonSwitchvoice)
	ON_BN_CLICKED(IDC_BUTTON_CONTROL_TYPE, OnButtonControlType)
	ON_BN_CLICKED(IDC_BUTTON_CONTROLSCAN, OnButtonControlscan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCELLULARDlg message handlers

BOOL CCELLULARDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    
	ReadIniFile();
    
	phonelst.rew();
    phonelst.clear_buf();

    areabase.rew();
    areabase.clear_buf();

    manufact.rew();
    manufact.clear_buf();

    m_EditPhone.LimitText(14);
	m_EditName.LimitText(30);
    m_EditName2.LimitText(30);
    m_EditArea.LimitText(30);
    m_EditOccupation.LimitText(30);
	m_EditSubject.LimitText(30);
    m_EditLanguage.LimitText(10);
    m_EditGender.LimitText(1);
    m_EditInfo1.LimitText(30);
    m_EditInfo2.LimitText(30);
    m_EditInfo3.LimitText(30);
    m_EditLinkedFile.LimitText(12);
    m_EditModemCalls.LimitText(1);
    m_EditFaxCalls.LimitText(1);
    m_EditAlertLevel.LimitText(1);
	m_EditRecordCall.LimitText(1);
    m_EditDtmf.LimitText(30);
    m_EditESN.LimitText(8);
	m_EditSCM.LimitText(2);

	bHoldCall = false;

	iCallCounter_0 = 0;
    iCallCounter_1 = 0;
    iCallCounter_2 = 0;
    iCallCounter_3 = 0;
    iCallCounter_4 = 0;
    iCallCounter_5 = 0;
    iCallCounter_6 = 0;
    iCallCounter_7 = 0;
    iCallCounter_8 = 0;
    iCallCounter_9 = 0;
    iCallCounter_total = 0;
    iCallCounter_record = 0;
    iCallCounter_fax = 0;
    iCallCounter_modem =0;
    iWaveFileNumber = 0;
    bMonitor = false;
	bAquire  = false;
    bGoReverseVoice = false;
    bGoForwardVoice = false;
    bVoiceForward = true;
 
	m_ProgressSignal.SetRange(0,25);
    // m_ProgressSignal.SetStep(1);


    m_Edit_Status.SetWindowText(" ");
    which_channel(); 
    
	bCancelThread=true;
   	bThreadClosed=true;


	return TRUE;  // return true  unless you set the focus to a control
}

void CCELLULARDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCELLULARDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCELLULARDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CCELLULARDlg::WriteToStatusBox(char *pchString)
{
	static int iCount = 0;
	iCount ++;

    char chTempString[50];
	char *pchTempString = chTempString;
	sprintf(pchTempString, "%d",iCount);
	SetDlgItemText(IDC_STATUS_TEXT,pchTempString);

	if(iCount >= 400)
    {
	    iCount = 0;
	    m_Edit_Status.SetWindowText(" ");
		m_Edit_Status.Clear();
    }

    m_Edit_Status.ReplaceSel(pchString,false);

 	char chNewLine[3];
	chNewLine[0]= 0x0D;
	chNewLine[1]= 0x0A;
	chNewLine[2]= 0x00;
    char *pchNewLine =  chNewLine;
	m_Edit_Status.ReplaceSel(pchNewLine,false);
}


void CCELLULARDlg::WriteToCallBox(char *pchString)
{
    m_EditCallList.ReplaceSel(pchString,false);

 	char chNewLine[3];
	chNewLine[0]= 0x0D;
	chNewLine[1]= 0x0A;
	chNewLine[2]= 0x00;
    char *pchNewLine =  chNewLine;
	m_EditCallList.ReplaceSel(pchNewLine,false);
}


void CCELLULARDlg::MyError(char *s)
{
	WriteDebugLog(s);
	MessageBeep(MB_ICONEXCLAMATION);
	MessageBox(s,"Program Error",MB_OK | MB_ICONHAND | MB_SETFOREGROUND);
}

void CCELLULARDlg::WriteDebugLog(char *chLogText)
{
    FILE *log;
	 if((log=fopen("DEBUG.LOG","a+"))==NULL)
	 {
	   	 // Error("Error opening DEBUG.LOG"); (No Error or infinate loop ..)
         return;
	 }
    else
    {
         fprintf(log,"%s\n",chLogText);
         fclose(log);
    }
}



void CCELLULARDlg::OnOK() 
{
	CreateMyThread();
}


void CCELLULARDlg::InitalizeDDI(void)
{
    WriteToStatusBox(" Initalizing DDI");
 
    // Set to 9600 BPS and Send DDI command to switch to 19200
	ComInit(iCommPortDDI);
    if(ComValid(iCommPortDDI))
    {
	    ComSetState(iCommPortDDI,9600,8,1,'N',SIGNAL_NONE);
   	    ComBufClear(iCommPortDDI,DIR_INC); 
	    ComBufClear(iCommPortDDI,DIR_OUT); 
		WriteToStatusBox(" DDI Comm Port Initalized @ 9600 BPS");
    }
    else 
    {
	    char chTempString[250];
        char *pchTempString = chTempString;
	    sprintf(pchTempString,"Com Port %d is not available or is in use",iCommPortDDI+1);
	    MyError(pchTempString);
		return;
    }
  
    ComDTROn(iCommPortDDI);  // set DTR
    ComRTSOn(iCommPortDDI);  // set RTS

	// Reset DDI
    WriteToStatusBox(" Sending DDI Reset Command (1X)");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'Y'); 

	// Reset DDI
    WriteToStatusBox(" Sending DDI Reset Command (2X)");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'Y'); 

	// Reset DDI
    WriteToStatusBox(" Sending DDI Reset Command (3X)");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'Y'); 


    // Switch DDI to 19200 bps
    WriteToStatusBox(" Sending DDI Command to increase BPS to 19200");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'F'); 

	WriteToStatusBox(" Switching DDI Comm Port to 19200 BPS");
    ComSetBaudRate(iCommPortDDI, 19200);
    // ComBufClear(iCommPortDDI,DIR_INC); 
    // ComBufClear(iCommPortDDI,DIR_OUT); 
    

    // WriteToStatusBox(" Sending DDI Command to display all control data");
	// ComWrite(iCommPortDDI,'-');
	// ComWrite(iCommPortDDI,'D');    // All Control Data
	
	WriteToStatusBox(" Sending DDI Command to display all control data changes only");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'d'); // Changes only


    WriteToStatusBox(" Sending DDI Command to show Channel on Goto");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'K'); 

    SetDDItoControlChannel();

	if(bReverseChannel == true)
    {
        SetDDItoReverse();
	}
	else
	{
        SetDDItoForward();
	}


}

void CCELLULARDlg::OnCancel() 
{
    #ifdef RECORD
	mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
    #endif

    bCancelThread=true;
	
	if(bThreadClosed==false)
    {
        SetDlgItemText(IDC_STATUS_TEXT,"Please wait while closing multithreaded engine.");
	    Sleep(3000);
    }

	if(bThreadClosed==false)
    {
        DWORD dwExitCode;
		
   	    if(GetExitCodeThread(hThread,&dwExitCode)==false)
		{
			MyError("Error in GetExitCodeThread(hThread)");
		}
        else
		{
			TerminateThread(hThread,dwExitCode);
		}
	}
	hThread = NULL;
	

    ComReset(iCommPortDDI);
	ComReset(iCommPortOS456);
	
	CDialog::OnCancel();
}

void CCELLULARDlg::CreateMyThread(void)
{
    #ifdef _DEBUG
    TRACE("CreateMyThread START\n");
    #endif

    
	DWORD dwThreadId;
    hThread = CreateThread((LPSECURITY_ATTRIBUTES) NULL, 0,
		(LPTHREAD_START_ROUTINE) ThreadProc,this,0,&dwThreadId);
	
	if(hThread == NULL)
	{
	    char chTempString[75];
	    char *pchTempString = chTempString;
	    sprintf(pchTempString,"Error %ld Creating Thread",GetLastError());
	    MyError(pchTempString);
	}
	else
	{
	    if(SetThreadPriority( hThread, THREAD_PRIORITY_NORMAL ) == 0)
		{
            MyError("Cound Not set thread to THREAD_PRIORITY_NORMAL");
		}
	}

    #ifdef _DEBUG
    TRACE("CreateMyThread END\n");
    #endif

}

DWORD CCELLULARDlg::ThreadProc(CCELLULARDlg *s)
{
    s->ProcessThread();
    return 0;
}

void CCELLULARDlg::ProcessThread(void)
{
    #ifdef _DEBUG
    TRACE("ProcessThread START\n");
    #endif

    bCancelThread=false;
   	bThreadClosed=false;

    InitalizeOS456();

   	InitalizeDDI();

    start:
 
    if(bCancelThread==true)
    {
		bThreadClosed=true;
	    return;
    }

	if(bHoldCall == false)
	{
	    GetDDIdata();
    }
	
	goto start;
}


void CCELLULARDlg::whattime(void)
{
    struct tm *today;
   
    time_t timer;
	timer = time(NULL);
	
	today = localtime( &timer );
	
    char timestatus[3]; // am/pm
    int hour1;
    if (today->tm_hour<12)
	{
	    strcpy(timestatus,"am");
	    hour1=today->tm_hour;
	    if (today->tm_hour==0){hour1=12;}
	}
    if (today->tm_hour>=12)
	{
	   strcpy(timestatus,"pm");
	   hour1=today->tm_hour - 12;
	   if (today->tm_hour==12){hour1=12;}
	}
    sprintf(timeis,"%02d:%02d:%02d%s",hour1,today->tm_min,today->tm_sec,timestatus);
    sprintf(dateis,"%02d-%02d-%02d",today->tm_mon+1,today->tm_mday,today->tm_year);
	sprintf(d_timeis,"%02d:%02d%c",hour1,today->tm_min,timestatus[0]);
}

void CCELLULARDlg::ZeroMemVariables(void)
{
    ::ZeroMemory (chDDIdataBuffer, sizeof (chDDIdataBuffer));
    ::ZeroMemory (chArea, sizeof (chArea));
}


void CCELLULARDlg::GetDDIdata(void)
{
    char i;
	if(ComRead(iCommPortDDI,&i)==TRUE)
	{
		 if(i==0x0D)
		 {
	         char chTempBuffer[500];
			 char *pchTempBuffer = chTempBuffer;
			 iBufferCounter=-1;
    		 whattime();
			 AnalyzeDDIdata();
	    	 if((chDDIdataBuffer[0]>=0x30) && (chDDIdataBuffer[0]<=0x39)) // Is first character a number ?
			 {
			      if((bRegistration==true) &&(bReverseChannel==false))
				  {
   					 sprintf(pchTempBuffer," %s %s\tRegistration [%c]\t%s",timeis,chPhoneNumber,chRegistration[0],chArea);
   				     WriteToStatusBox(pchTempBuffer);
			  
					 /*
					 if(logit==true)
					 {
				         fprintf(log," %s %s  Registration [%c]  %s\n",timeis,chPhoneNumber,chRegistration[0],chArea);
					 }
					 */
				  }

		          if(bDisconnect==true)
				  {
           			  sprintf(pchTempBuffer," %s %s\tDisconnect\t%s",timeis,chPhoneNumber,chArea);

					  WriteToStatusBox(pchTempBuffer);
			          
					  /*
					  if(logit==true)
				      {
				          fprintf(log," %s %s  Disconnect        %s\n",timeis,chPhoneNumber,chArea);
				      }
					  */
			      }

          		  if(bPaging==true)
				  {
   			 		  sprintf(pchTempBuffer," %s %s\tPaging\t\t%s",timeis,chPhoneNumber,chArea);
					  WriteToStatusBox(pchTempBuffer);
			
					  /*
					  if(logit==true)
			          {
				          fprintf(log," %s %s  Paging            %s\n",timeis,chPhoneNumber,chArea);
			          }
					  */
				  }

		          if((bReorder==true)&&(bLockOut==false))
				  {
   			          sprintf(pchTempBuffer," %s %s\tReorder\t\t%s",timeis,chPhoneNumber,chArea);

					  WriteToStatusBox(pchTempBuffer);
			          
					  /*
					  if(logit==true)
					  {
				          fprintf(log," %s %s  Reorder           System to busy - Try later.\n",timeis,chPhoneNumber);
					  }
					  */
				  }

		          if((bGoto ==true)&&(bLockOut==true))
				  {
			          sprintf(pchTempBuffer," %s %s\tLocked Out Goto\t%s",timeis,chPhoneNumber,chArea);
					  WriteToStatusBox(pchTempBuffer);
				  }

		          if((bGoto ==true)&&(bLockOut==false))
				  {
   					  sprintf(pchTempBuffer," %s %s\tGoto %s\t%s",timeis,chPhoneNumber,chGoFrequency,chArea);

					  WriteToStatusBox(pchTempBuffer);

				      switch_to_voice();
				  } 

		  } // End if (is it a number ?)

		  if(((chDDIdataBuffer[0]<0x30)||(chDDIdataBuffer[0]>0x39)) || (bReverseChannel==true) ) // Is first character NOT a number ?
		  {
		     if((bAllPaging==false)&&(bAllRegistration==false)&&(chDDIdataBuffer[0] != 0x00)&&(chDDIdataBuffer[1] != 0x00)&&(chDDIdataBuffer[0] != 0x20))
			 {
				 sprintf(pchTempBuffer," %s",chDDIdataBuffer);
				 WriteToStatusBox(pchTempBuffer);

				 /*
				 if(logit==true)
				 {
				    fprintf(log," %s\n",chDDIdataBuffer);
				 }
				 */
			 }
             
		  } // end if (if first character NOT a number?)
		  ZeroMemVariables();
		}	  // End if (i>1)
	
	    if((i>=0x20) && (i <=0x7A))
		{
	       iBufferCounter++;
	       chDDIdataBuffer[iBufferCounter] = (char) i;;
		}
	}
}


void CCELLULARDlg::AnalyzeDDIdata(void)
{
    ossignalstr();

    bRegistration  = false; // reset registration flag
    bDisconnect    = false; // reset disconnect flag
    bPaging        = false; // reset paging flag
    bGoto          = false; // reset Goto flag
    bVoiceGoto     = false; // reset voice GoTo Flag
    bReorder       = false; // reset bReorder flag
    bLockOut       = false; // reset bLockOut flag
    /* personal       = false; // reset personal flag */
    bAllRegistration = false; // reset all registration flag
    bAllPaging     = false; // reset all paging flag

    char chTempBuffer[500];
    char *pchTempBuffer = chTempBuffer;

    if((chDDIdataBuffer[0]<0x30) || (chDDIdataBuffer[0]>0x39)) // Is first character NOT a number ?
	{
		// Check if Goto Command from Voice Channel(check for "GoT")
		if ((chDDIdataBuffer[0]=='G')&&(chDDIdataBuffer[1]=='o')&&(chDDIdataBuffer[2]=='T') )
		{
			bVoiceGoto = true;
			
			// Jump to which channel
			chChannel[0] = chDDIdataBuffer[5];
			chChannel[1] = chDDIdataBuffer[6];
			chChannel[2] = chDDIdataBuffer[7];
			chChannel[3] = chDDIdataBuffer[8];

			// Jump to which frequency
			chGoFrequency[0] = chDDIdataBuffer[10];// 8
			chGoFrequency[1] = chDDIdataBuffer[11];// 8
			chGoFrequency[2] = chDDIdataBuffer[12];// 0
			chGoFrequency[3] = chDDIdataBuffer[13];// .
			chGoFrequency[4] = chDDIdataBuffer[14];// 5
			chGoFrequency[5] = chDDIdataBuffer[15];// 0
			chGoFrequency[6] = chDDIdataBuffer[16];// 0
			
            WriteToStatusBox(" At Goto2 Subroutine ");
			
			/*
			if(logit==true)
			{
				fprintf(log,"At Goto2 Subroutine \n");
			}
			*/
			goto END;
		}

		//  Check if "?" or ">"
		if((chDDIdataBuffer[0]==0x3F)||(chDDIdataBuffer[0]==0x3E) )
		{
			ZeroMemVariables();
			goto END;
		}

		//  Check if Voice
		if((chDDIdataBuffer[0]=='V')&&(chDDIdataBuffer[1]=='o')&&(chDDIdataBuffer[2]=='i') )
		{
			WriteToStatusBox(" DDI Successfully set to Voice Mode");
			ZeroMemVariables();
			goto END;
		}

		//  Check if E1 thru EE but not E0 (Reserved, Ignore)
		if((chDDIdataBuffer[0]=='E')&&(chDDIdataBuffer[1]!='0'))
		{
			ZeroMemVariables();
			goto END;
		}

		//  Check if F1 thru FF but not F0 (Reserved, Ignore)
		if((chDDIdataBuffer[0]=='E')&&(chDDIdataBuffer[1]!='0'))
		{
			ZeroMemVariables();
			goto END;
		}

		//  Check if D2 thru DB but not any other Dx (Reserved, Ignore)
		if((chDDIdataBuffer[0]=='D')&&(chDDIdataBuffer[1]=='2' || '3' || '4' || '5' || '6'|| '7' || '8' || '9' || 'A' || 'B'))
		{
			ZeroMemVariables();
			goto END;
		}

		//  Check if System Requested Registration
		if((chDDIdataBuffer[6]=='R')&&(chDDIdataBuffer[7]=='e')&&(chDDIdataBuffer[8]=='g') )
		{
			bAllRegistration = true;

			// All registration of Which area code
			chAllRegistrations[0]=chDDIdataBuffer[1];
			chAllRegistrations[1]=chDDIdataBuffer[2];
			chAllRegistrations[2]=chDDIdataBuffer[3];

			// What type of registration
			chRegistrationType[0]=chDDIdataBuffer[19];

			goto END;
		}
		
		//  Check if System Page
		if((chDDIdataBuffer[6]=='P')&&(chDDIdataBuffer[7]=='a')&&(chDDIdataBuffer[8]=='g'))
		{
			bAllPaging = true;

			// All Page of Which area code
			chAllPaging[0]=chDDIdataBuffer[1];
			chAllPaging[1]=chDDIdataBuffer[2];
			chAllPaging[2]=chDDIdataBuffer[3];

		    goto END;
		}
		
		//  Check if Control
		if((chDDIdataBuffer[0]=='C')&&(chDDIdataBuffer[1]=='o')&&(chDDIdataBuffer[2]=='n') )
		{
            WriteToStatusBox(" DDI Successfully set to Control Mode");
			ZeroMemVariables();
			goto END;
		}
		
		//  Check if Forward
		if((chDDIdataBuffer[0]=='F')&&(chDDIdataBuffer[1]=='o')&&(chDDIdataBuffer[2]=='r'))
		{
            WriteToStatusBox(" DDI Successfully set to Forward Mode");			
			ZeroMemVariables();
			goto END;
		}

		//  Check if Reverse
		if((chDDIdataBuffer[0]=='R')&&(chDDIdataBuffer[1]=='e')&&(chDDIdataBuffer[2]=='v') )
		{
			WriteToStatusBox(" DDI Successfully set to Reverse Mode");			
			ZeroMemVariables();
			goto END;
		}

   	    //  Check if System ID & DCC   on Control Channel
		if((chDDIdataBuffer[0]=='S')&&(chDDIdataBuffer[1]=='I')&&(chDDIdataBuffer[2]=='D'))
		{
			char chSID[5];
			::ZeroMemory (chSID, sizeof (chSID));
            char chDCC[2];
			::ZeroMemory (chDCC, sizeof (chDCC));

			chSID[0]=chDDIdataBuffer[4];
			chSID[1]=chDDIdataBuffer[5];
			chSID[2]=chDDIdataBuffer[6];
			chSID[3]=chDDIdataBuffer[7];
			SetDlgItemText(IDC_TEXT_SID,chSID);

    		if((chDDIdataBuffer[9]=='D')&&(chDDIdataBuffer[10]=='C')&&(chDDIdataBuffer[11]=='C') )
			{
			    chDCC[0]=chDDIdataBuffer[4];
			    SetDlgItemText(IDC_TEXT_DCC,chDCC);
			}
			
			/* UpdateChannelStats(); */
			ZeroMemVariables();
			goto END;
		}

	 	// Check for NPC=xx on Control Channel
		if((chDDIdataBuffer[0]=='N')&&(chDDIdataBuffer[1]=='P')&&(chDDIdataBuffer[2]=='C'))
		{
			char chNPC[3];
			::ZeroMemory (chNPC, sizeof (chNPC));
			chNPC[0]=chDDIdataBuffer[4];
			chNPC[1]=chDDIdataBuffer[5];
			
			SetDlgItemText(IDC_TEXT_NPC,chNPC);
			ZeroMemVariables();
			goto END;
		}

		// Check for NAC=xx on Control Channel
		if((chDDIdataBuffer[0]=='N')&&(chDDIdataBuffer[1]=='A')&&(chDDIdataBuffer[2]=='C'))
		{
			char chNAC[3];
			::ZeroMemory (chNAC, sizeof (chNAC));
			chNAC[0]=chDDIdataBuffer[4];
			chNAC[1]=chDDIdataBuffer[5];
			
			SetDlgItemText(IDC_TEXT_NAC,chNAC);
			ZeroMemVariables();
			goto END;
		}

	    //  Check if RegInc=xxxx on Control Channel
		if((chDDIdataBuffer[0]=='R')&&(chDDIdataBuffer[1]=='e')&&(chDDIdataBuffer[2]=='g')&&(chDDIdataBuffer[3]=='I') )
		{
			char chRegInc[6];
			::ZeroMemory (chRegInc, sizeof (chRegInc));
			chRegInc[0]=chDDIdataBuffer[7];
			chRegInc[1]=chDDIdataBuffer[8];
			chRegInc[2]=chDDIdataBuffer[9];
			chRegInc[3]=chDDIdataBuffer[10];
			
			SetDlgItemText(IDC_TEXT_REGINC,chRegInc);
			ZeroMemVariables();
			goto END;
		}

        //  Check if Modes=XXXXXXX  on Control Channel
		if((chDDIdataBuffer[0]=='M')&&(chDDIdataBuffer[1]=='o')&&(chDDIdataBuffer[2]=='d'))
		{
			if((chDDIdataBuffer[6]=='S')|| (chDDIdataBuffer[7]=='S')||(chDDIdataBuffer[8]=='S')||(chDDIdataBuffer[9]=='S')||
				(chDDIdataBuffer[10]=='S')|| (chDDIdataBuffer[11]=='S')|| (chDDIdataBuffer[12]=='S'))
			{
			    SetDlgItemText(IDC_TEXT_MODES_S,"YES");
			}
			else
			{
			    SetDlgItemText(IDC_TEXT_MODES_S,"NO");
			}

			if((chDDIdataBuffer[6]=='E')|| (chDDIdataBuffer[7]=='E')||(chDDIdataBuffer[8]=='E')||(chDDIdataBuffer[9]=='E')||
				(chDDIdataBuffer[10]=='E')|| (chDDIdataBuffer[11]=='E')|| (chDDIdataBuffer[12]=='E'))
			{
			    SetDlgItemText(IDC_TEXT_MODES_E,"YES");
			}
			else
			{
			    SetDlgItemText(IDC_TEXT_MODES_E,"NO");
			}

			if((chDDIdataBuffer[6]=='H')|| (chDDIdataBuffer[7]=='H')||(chDDIdataBuffer[8]=='H')||(chDDIdataBuffer[9]=='H')||
				(chDDIdataBuffer[10]=='H')|| (chDDIdataBuffer[11]=='H')|| (chDDIdataBuffer[12]=='H'))
			{
			    SetDlgItemText(IDC_TEXT_MODES_H,"YES");
			}
			else
			{
			    SetDlgItemText(IDC_TEXT_MODES_H,"NO");
			}

			if((chDDIdataBuffer[6]=='R')|| (chDDIdataBuffer[7]=='R')||(chDDIdataBuffer[8]=='R')||(chDDIdataBuffer[9]=='R')||
				(chDDIdataBuffer[10]=='R')|| (chDDIdataBuffer[11]=='R')|| (chDDIdataBuffer[12]=='R'))
			{
			    SetDlgItemText(IDC_TEXT_MODES_R,"YES");
			}
			else
			{
			    SetDlgItemText(IDC_TEXT_MODES_R,"NO");
			}

			if((chDDIdataBuffer[6]=='D')|| (chDDIdataBuffer[7]=='D')||(chDDIdataBuffer[8]=='D')||(chDDIdataBuffer[9]=='D')||
				(chDDIdataBuffer[10]=='D')|| (chDDIdataBuffer[11]=='D')|| (chDDIdataBuffer[12]=='D'))
			{
			    SetDlgItemText(IDC_TEXT_MODES_D,"YES");
			}
			else
			{
			    SetDlgItemText(IDC_TEXT_MODES_D,"NO");
			}

			if((chDDIdataBuffer[6]=='F')|| (chDDIdataBuffer[7]=='F')||(chDDIdataBuffer[8]=='F')||(chDDIdataBuffer[9]=='F')||
				(chDDIdataBuffer[10]=='F')|| (chDDIdataBuffer[11]=='F')|| (chDDIdataBuffer[12]=='F'))
			{
			    SetDlgItemText(IDC_TEXT_MODES_F,"YES");
			}
			else
			{
			    SetDlgItemText(IDC_TEXT_MODES_F,"NO");
			}

			if((chDDIdataBuffer[6]=='C')|| (chDDIdataBuffer[7]=='C')||(chDDIdataBuffer[8]=='C')||(chDDIdataBuffer[9]=='C')||
				(chDDIdataBuffer[10]=='C')|| (chDDIdataBuffer[11]=='C')|| (chDDIdataBuffer[12]=='C'))
			{
			    SetDlgItemText(IDC_TEXT_MODES_C,"YES");
			}
			else
			{
			    SetDlgItemText(IDC_TEXT_MODES_C,"NO");
			}
			
			ZeroMemVariables();
			goto END;
		
        }
		

			//check for Ring Phone on Voice Channel
			if((chDDIdataBuffer[0]=='R')&&(chDDIdataBuffer[1]=='i')&&(chDDIdataBuffer[2]=='n') )
			{
			      // ring = true;
                  WriteToStatusBox(" Incoming Ring on Voice Channel");
				  ZeroMemVariables();
			      goto END;
			}


	}


  if((chDDIdataBuffer[0]>=0x30) && (chDDIdataBuffer[0]<=0x39)) // Is first character a number ?
  {
      // Change phone number to (xxx) xxx-xxxx format & save in chPhoneNumber array
      chPhoneNumber[0] =chDDIdataBuffer[9];   // Area Code
      chPhoneNumber[1] =chDDIdataBuffer[10];// 6
      chPhoneNumber[2] =chDDIdataBuffer[11];// 1
      chPhoneNumber[3] =chDDIdataBuffer[12];// 9
      chPhoneNumber[4] =chDDIdataBuffer[13];
      chPhoneNumber[5] =chDDIdataBuffer[8];  // Space
      chPhoneNumber[6] =chDDIdataBuffer[0];  // prefix
      chPhoneNumber[7] =chDDIdataBuffer[1];
      chPhoneNumber[8] =chDDIdataBuffer[2];
      chPhoneNumber[9] =chDDIdataBuffer[3]; // -
      chPhoneNumber[10]=chDDIdataBuffer[4]; // suffix
      chPhoneNumber[11]=chDDIdataBuffer[5];
      chPhoneNumber[12]=chDDIdataBuffer[6];
      chPhoneNumber[13]=chDDIdataBuffer[7];


      chAreaCode[0] = chDDIdataBuffer[10];
      chAreaCode[1] = chDDIdataBuffer[11];
      chAreaCode[2] = chDDIdataBuffer[12];

      strcpy(phonelst.Dphone,chPhoneNumber);
      search();
      
      areabase.Anumber[0] =chPhoneNumber[1];// 6
      areabase.Anumber[1] =chPhoneNumber[2];// 1
      areabase.Anumber[2] =chPhoneNumber[3];// 9
      areabase.Anumber[3] =chPhoneNumber[6];  // prefix
      areabase.Anumber[4] =chPhoneNumber[7];
      areabase.Anumber[5] = 0x00;
      areabase.Anumber[6] = 0x00;
      areabase.Anumber[7] = 0x00;
      areabase.Anumber[8] = 0x00;
      areabase.Anumber[9] = 0x00;
      areabase.Anumber[10] = 0x00;
      areabase.Anumber[11] = 0x00;
      areabase.Anumber[12] = 0x00;
      areabase.Anumber[13] = 0x00;

      if((areabase.find()== IM_OK))
	  {
	      strcpy(chArea,areabase.Aarea);
	 	  goto OK1;
	  }
      else
	  {
	      areabase.Anumber[0] =chPhoneNumber[1];// 6
	      areabase.Anumber[1] =chPhoneNumber[2];// 1
	      areabase.Anumber[2] =chPhoneNumber[3];// 9
	      areabase.Anumber[3] =chPhoneNumber[6];  // prefix
	      areabase.Anumber[4] = 0x00;
	      areabase.Anumber[5] = 0x00;
	      areabase.Anumber[6] = 0x00;
	      areabase.Anumber[7] = 0x00;
	      areabase.Anumber[8] = 0x00;
	      areabase.Anumber[9] = 0x00;
	      areabase.Anumber[10] = 0x00;
	      areabase.Anumber[11] = 0x00;
	      areabase.Anumber[12] = 0x00;
	      areabase.Anumber[13] = 0x00;

          if((areabase.find()== IM_OK))
	      {
		      strcpy(chArea,areabase.Aarea);
			  goto OK1;
	  	  }
          else
	      {
		      areabase.Anumber[0] =chPhoneNumber[1];// 6
		      areabase.Anumber[1] =chPhoneNumber[2];// 1
		      areabase.Anumber[2] =chPhoneNumber[3];// 9
		      areabase.Anumber[3] = 0x00;
		      areabase.Anumber[4] = 0x00;
		      areabase.Anumber[5] = 0x00;
		      areabase.Anumber[6] = 0x00;
		      areabase.Anumber[7] = 0x00;
		      areabase.Anumber[8] = 0x00;
		      areabase.Anumber[9] = 0x00;
		      areabase.Anumber[10] = 0x00;
		      areabase.Anumber[11] = 0x00;
		      areabase.Anumber[12] = 0x00;
		      areabase.Anumber[13] = 0x00;

    		  if((areabase.find()== IM_OK))
			  {
			      strcpy(chArea,areabase.Aarea);
				  goto OK1;
			  }
		      else
			  {
			      areabase.Anumber[0] =chPhoneNumber[1];// 6
			      areabase.Anumber[1] =chPhoneNumber[2];// 1
			      areabase.Anumber[2] = 0x00;
			      areabase.Anumber[3] = 0x00;
			      areabase.Anumber[4] = 0x00;
			      areabase.Anumber[5] = 0x00;
			      areabase.Anumber[6] = 0x00;
			      areabase.Anumber[7] = 0x00;
			      areabase.Anumber[8] = 0x00;
			      areabase.Anumber[9] = 0x00;
			      areabase.Anumber[10] = 0x00;
			      areabase.Anumber[11] = 0x00;
			      areabase.Anumber[12] = 0x00;
			      areabase.Anumber[13] = 0x00;

			      if((areabase.find()== IM_OK))
				  {
				      strcpy(chArea,areabase.Aarea);
					  goto OK1;
				  }
			      else
				  {
				      strcpy(chArea,"Not Found");
				  }
      		  }
   
          }
      }

  
	OK1:


   // Check if Registration (check for "Reg")
   if((chDDIdataBuffer[15]==0x52) && (chDDIdataBuffer[16]=='e') && (chDDIdataBuffer[17]==0x67) )
   {
 	   chRegistration[0] = chDDIdataBuffer[28];
	   if(bReverseChannel==false)
	   {
		   bRegistration = true;
	   }
	
	   if((chDDIdataBuffer[32]=='S') && (chDDIdataBuffer[33]=='C') && (chDDIdataBuffer[34]=='M'))
	   {
	 	  
		   phonelst.Dscm[0]=chDDIdataBuffer[36];
		   phonelst.Dscm[1]=chDDIdataBuffer[37];

		   phonelst.Desn[0]=chDDIdataBuffer[43];
		   phonelst.Desn[1]=chDDIdataBuffer[44];
		   phonelst.Desn[2]=chDDIdataBuffer[45];
		   phonelst.Desn[3]=chDDIdataBuffer[46];
		   phonelst.Desn[4]=chDDIdataBuffer[47];
		   phonelst.Desn[5]=chDDIdataBuffer[48];
		   phonelst.Desn[6]=chDDIdataBuffer[49];
		   phonelst.Desn[7]=chDDIdataBuffer[50];

		   if(phonelst.Darea[0]==0x00)
		   {
			   strcpy(phonelst.Darea,chArea);
		   }
		  
		   if(phonelst.Dtype[0]==0x00)
		   {
			   phonelst.Dtype[0]=chChannelBand[0];
		   }

    	   if(phonelst.Dphone[0]==0x00)
		   {
			   strcpy(phonelst.Dphone,chPhoneNumber);
			   insert_record();
			   bNewTest=false;
			   bNewNumber=true;
		   }
		   
		   if(bNewNumber==false)
		   {
		 	   strcpy(chWhereErrorOccured,"Line 2150");
			   amend_record();
			   bAmend=false;
			   bNewTest=false;
		   }

		      char chTempString[400];
			  char *pchTempString = chTempString;
			  sprintf(pchTempString," [%s]  [SC = %s]  [ESN = %s]",chPhoneNumber,phonelst.Dscm,phonelst.Desn);
			  WriteToCallBox(pchTempString);
		}
		
		   goto END;
	//}
  }
	   
		 // Check if Reorder (check for "Reo")
		if((chDDIdataBuffer[15]=='R') && (chDDIdataBuffer[16]=='e') && (chDDIdataBuffer[17]=='o'))
		{
			TRACE("REORDER\n");
			bReorder = true;
			goto END;
		}
		
		// Check if Control channel Disconnect (check for "Di")
		if((chDDIdataBuffer[15]=='D') && (chDDIdataBuffer[16]=='i') )
		{
			TRACE("DISCONNECT\n");
			bDisconnect = true;
			goto END;
		}

		// Check if Paging (check for "Pa")
		if((chDDIdataBuffer[15]=='P')&&(chDDIdataBuffer[16]=='a'))
		{
			TRACE("PAGING\n");
			bPaging = true;
			goto END;
		}

		// Check if Goto Command (check for "Go")
		if((chDDIdataBuffer[15]=='G')&&(chDDIdataBuffer[16]=='o'))
		{
			bGoto = true;

			// Jump to which channel
			chChannel[0] = chDDIdataBuffer[20];
			chChannel[1] = chDDIdataBuffer[21];
			chChannel[2] = chDDIdataBuffer[22];
			chChannel[3] = chDDIdataBuffer[23];


			// Jump to which frequency
			chGoFrequency[0] = chDDIdataBuffer[25];// 8
			chGoFrequency[1] = chDDIdataBuffer[26];// 8
			chGoFrequency[2] = chDDIdataBuffer[27];// 0
			chGoFrequency[3] = chDDIdataBuffer[28];// .
			chGoFrequency[4] = chDDIdataBuffer[29];// 5
			chGoFrequency[5] = chDDIdataBuffer[30];// 0
			chGoFrequency[6] = '0';

		    
			if(bMonitor == true)
			{
			    /*
				if((phonelst.DRecordCall[0]!='Y'))
				{
					bLockOut=true;
				}
				*/

				if((phonelst.Dalert[0]=='5'))
				{
				    WriteToStatusBox(" Monitor Mode: Locked Out Level 4 Call");
					bLockOut=true;
				}

				if((phonelst.Dalert[0]=='3'))
				{
				    WriteToStatusBox(" Monitor Mode: Locked Out Level 3 Call");
					bLockOut=true;
				}
			    if((phonelst.Dalert[0]=='2'))
				{
				    WriteToStatusBox(" Monitor Mode: Locked Out Level 2 Call");
					bLockOut=true;
				}
			    if((phonelst.Dalert[0]=='1'))
				{
				    WriteToStatusBox(" Monitor Mode: Locked Out Level 1 Call");
					bLockOut=true;
				}
			    if((phonelst.Dalert[0]=='0'))
				{
				    WriteToStatusBox(" Monitor Mode: Locked Out Level 0 Call");
					bLockOut=true;
				}
			    if((phonelst.Dalert[0]== 0x00))
				{
				    WriteToStatusBox(" Monitor Mode: Locked Out Level 0 Call");
					bLockOut=true;
				}

			}
		    
			if(bAquire == true)
			{
			    if((phonelst.Dalert[0]=='3'))
				{
				    bLockOut=true;
				}
			    if((phonelst.Dalert[0]=='2'))
				{
				    bLockOut=true;
				}
			    if((phonelst.Dalert[0]=='1'))
				{
				    bLockOut=true;
				}
			}
			
		 }
     }

  END:


  TRACE("%s\n",chDDIdataBuffer);
  return;
}





void CCELLULARDlg::OnErrspaceEditStatus() 
{
	m_Edit_Status.Clear();
    WriteToStatusBox(" Status Edit Box Out of Memory");
}


void CCELLULARDlg::amend_record(void)
{
    show_database();
    if((phonelst.amend())!= IM_OK)
	{
	    if((phonelst.amend())!= IM_OK)
		{
		     if((phonelst.amend())!= IM_OK)
			 {
                char chTempstring[500];
				char *pchTempstring = chTempstring;
				sprintf(pchTempstring,"ERROR : In phonelst.amend routine\nWhere : %s\n"
        					          "number = %s\n phonelst.number = %s\n",chWhereErrorOccured,chPhoneNumber,phonelst.Dphone);
                MyError(pchTempstring);
				// WriteDebugLog(pchTempstring);
			 }
		}
    }
}

void CCELLULARDlg::insert_record(void)
{
    show_database();
    if((phonelst.insert())!= IM_OK)
	{
	    if((phonelst.insert())!= IM_OK)
		{
		     if((phonelst.insert())!= IM_OK)
			 {
                char chTempstring[500];
				char *pchTempstring = chTempstring;
				sprintf(pchTempstring,"ERROR : In phonelst.insert routine\n"
        					          "number = %s\nphonelst.number = %s\n",chPhoneNumber,phonelst.Dphone);
                MyError(pchTempstring);

			 }
		 }
	}
}


void CCELLULARDlg::show_database(void)
{
    m_EditPhone.SetWindowText(phonelst.Dphone);
	m_EditName.SetWindowText(phonelst.Dname);
    m_EditName2.SetWindowText(phonelst.Dname2);
    m_EditArea.SetWindowText(phonelst.Darea);
    m_EditOccupation.SetWindowText(phonelst.Doccupation);
	m_EditSubject.SetWindowText(phonelst.Dsubject);
    m_EditGender.SetWindowText(phonelst.Dgender);
    m_EditLanguage.SetWindowText(phonelst.Dlanguage);
    m_EditDtmf.SetWindowText(phonelst.DDtmfDigits);
    m_EditESN.SetWindowText(phonelst.Desn);
    m_EditSCM.SetWindowText(phonelst.Dscm);

    char chTempString[250];
	char *pchTempString = chTempString;

	sprintf(pchTempString,"%s %s",phonelst.DLastCallDate, phonelst.DLastCallTime);
    m_EditLastCall.SetWindowText(pchTempString);

	sprintf(pchTempString,"%d",phonelst.DTotalCalls);
	m_Edit_TotalCalls.SetWindowText(pchTempString);

    m_EditInfo1.SetWindowText(phonelst.Dinfo1);
    m_EditInfo2.SetWindowText(phonelst.Dinfo2);
    m_EditInfo3.SetWindowText(phonelst.Dinfo3);
    m_EditLinkedFile.SetWindowText(phonelst.DLinkedFile);

 	if(phonelst.Dscm[1]=='0'||'1'||'2'||'3'||'4'||'5'||'6'||'7'||'8'||'9'||'A'||'B'||'C'||'D'||'E'||'F')
	{
        show_scm();
		m_EditPhoneModel.SetWindowText(chSCMstring);
	}

    if(phonelst.Desn[0]!=0x00)
	{
		 manufact.hex[0]=phonelst.Desn[0];
		 manufact.hex[1]=phonelst.Desn[1];
		 if((manufact.find()== IM_OK))
		 {
              m_EditPhoneType.SetWindowText(manufact.man);
		 }
		 if((manufact.find()!= IM_OK))
		 {
              m_EditPhoneType.SetWindowText("Unknown Phone Manufacture");
		 }
	 }
	else
	{
    	m_EditPhoneType.SetWindowText(" ");
	}

	m_EditModemCalls.SetWindowText(phonelst.Dmodem);
    m_EditFaxCalls.SetWindowText(phonelst.Dfax);
    m_EditAlertLevel.SetWindowText(phonelst.Dalert);
	m_EditRecordCall.SetWindowText(phonelst.DRecordCall);
    m_EditChannelAB.SetWindowText(phonelst.Dtype);
}


void CCELLULARDlg::search(void)
{
    if((phonelst.find()== IM_OK))
	{
	    bNewNumber=false;

  
		show_database();

     	/*
		if(phonelst.DLinkedFile[0]!=0x00)
		{
		    get_linkfile();
		}
        */
	 }
     else
	 {
	     if(bNewTest==true)
		 {
		    bNewNumber = true;
		    return;
		 }

	     phonelst.clear_buf();
	     bAmend=false;
	 }
}


void CCELLULARDlg::which_channel(void)
{
	bool bOK = false;

	TRACE("which_channel chFrequency = %s\n",chFrequency);

	strcpy(chGoFrequency,chFrequency);
	ConvertGoFrequency();


    //// BAND 'A' //////////////////////////////////
	////////////////////////////////////////////////
	if(strcmp(chFrequency,"879.990")==0)  // FORWARD
	{
		chChannelBand[0]='A';
    	bReverseChannel = false;
		strcpy(chChannel,"333");
        strcpy(chReverseChannel,"834.990"); 
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.990")==0) //REVERSE
	{
		chChannelBand[0]='A';
    	bReverseChannel = true;
		strcpy(chChannel,"333");
        strcpy(chForwardChannel,"879.990");
        bOK	= true;
	}
    
    ////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.960")==0)  // FORWARD
	{
		chChannelBand[0]='A';
    	bReverseChannel = false;
		strcpy(chChannel,"332");
        strcpy(chReverseChannel,"834.960");
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.960")==0)//REVERSE
	{
		chChannelBand[0]='A';
    	bReverseChannel = true;
		strcpy(chChannel,"332");
        strcpy(chForwardChannel,"879.960");
        bOK	= true;
	}

    ////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.930")==0)  // FORWARD
	{
		chChannelBand[0]='A';
    	bReverseChannel = false;
		strcpy(chChannel,"331");
        strcpy(chReverseChannel,"834.930");
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.930")==0)//REVERSE
	{
		chChannelBand[0]='A';
    	bReverseChannel = true;
		strcpy(chChannel,"331");
        strcpy(chForwardChannel,"879.930");
        bOK	= true;
	}
	
    ////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.900")==0)  // FORWARD
	{
		strcpy(chChannel,"330");
        strcpy(chReverseChannel,"834.900");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.900")==0)//REVERSE
	{
		strcpy(chChannel,"330");
        strcpy(chForwardChannel,"879.900");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}
	

    ////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.870")==0)  // FORWARD
	{
		strcpy(chChannel,"329");
        strcpy(chReverseChannel,"834.870");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.870")==0)//REVERSE
	{
		strcpy(chChannel,"329");
        strcpy(chForwardChannel,"879.870");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}

	
    ////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.840")==0)  // FORWARD
	{
		strcpy(chChannel,"328");
        strcpy(chReverseChannel,"834.840");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.840")==0)//REVERSE
	{
		strcpy(chChannel,"328");
        strcpy(chForwardChannel,"879.840");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}


    ////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.810")==0)  // FORWARD
	{
		strcpy(chChannel,"327");
        strcpy(chReverseChannel,"834.810");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.810")==0)//REVERSE
	{
		strcpy(chChannel,"327");
        strcpy(chForwardChannel,"879.810");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}



    ////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.780")==0)  // FORWARD
	{
		strcpy(chChannel,"326");
        strcpy(chReverseChannel,"834.780");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.780")==0)//REVERSE
	{
		strcpy(chChannel,"326");
        strcpy(chForwardChannel,"879.780");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}

    
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.750")==0)  // FORWARD
	{
		strcpy(chChannel,"325");
        strcpy(chReverseChannel,"834.750");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.750")==0)//REVERSE
	{
		strcpy(chChannel,"325");
        strcpy(chForwardChannel,"879.750");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}

	
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.720")==0)  // FORWARD
	{
		strcpy(chChannel,"324");
        strcpy(chReverseChannel,"834.720");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.720")==0)//REVERSE
	{
		strcpy(chChannel,"324");
        strcpy(chForwardChannel,"879.720");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}

	
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.690")==0)  // FORWARD
	{
		strcpy(chChannel,"323");
        strcpy(chReverseChannel,"834.690");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.690")==0)//REVERSE
	{
		strcpy(chChannel,"323");
        strcpy(chForwardChannel,"879.690");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}


	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.660")==0)  // FORWARD
	{
		strcpy(chChannel,"322");
        strcpy(chReverseChannel,"834.660");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.660")==0)//REVERSE
	{
		strcpy(chChannel,"322");
        strcpy(chForwardChannel,"879.660");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}

	

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.630")==0)  // FORWARD
	{
		strcpy(chChannel,"321");
        strcpy(chReverseChannel,"834.630");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.630")==0)//REVERSE
	{
		strcpy(chChannel,"321");
        strcpy(chForwardChannel,"879.630");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}


	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.600")==0)  // FORWARD
	{
		strcpy(chChannel,"320");
        strcpy(chReverseChannel,"834.600");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.600")==0)//REVERSE
	{
		strcpy(chChannel,"320");
        strcpy(chForwardChannel,"879.600");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}
	

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.570")==0)  // FORWARD
	{
		strcpy(chChannel,"319");
        strcpy(chReverseChannel,"834.570");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.570")==0)//REVERSE
	{
		strcpy(chChannel,"319");
        strcpy(chForwardChannel,"879.570");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}
	
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.540")==0)  // FORWARD
	{
		strcpy(chChannel,"318");
        strcpy(chReverseChannel,"834.540");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.540")==0)//REVERSE
	{
		strcpy(chChannel,"318");
        strcpy(chForwardChannel,"879.540");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}
	
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.510")==0)  // FORWARD
	{
		strcpy(chChannel,"317");
        strcpy(chReverseChannel,"834.510");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.510")==0)//REVERSE
	{
		strcpy(chChannel,"317");
        strcpy(chForwardChannel,"879.510");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}
	

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.480")==0)  // FORWARD
	{
		strcpy(chChannel,"316");
        strcpy(chReverseChannel,"834.480");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.480")==0)//REVERSE
	{
		strcpy(chChannel,"316");
        strcpy(chForwardChannel,"879.480");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}
	

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.450")==0)  // FORWARD
	{
		strcpy(chChannel,"315");
        strcpy(chReverseChannel,"834.450");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.450")==0)//REVERSE
	{
		strcpy(chChannel,"315");
        strcpy(chForwardChannel,"879.450");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.420")==0)  // FORWARD
	{
		strcpy(chChannel,"314");
        strcpy(chReverseChannel,"834.420");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.420")==0)//REVERSE
	{
		strcpy(chChannel,"314");
        strcpy(chForwardChannel,"879.420");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"879.390")==0)  // FORWARD
	{
		strcpy(chChannel,"313");
        strcpy(chReverseChannel,"834.390");
		bReverseChannel = false;
		chChannelBand[0]='A';
        bOK	= true;
	}

    if(strcmp(chFrequency,"834.390")==0)//REVERSE
	{
		strcpy(chChannel,"313");
        strcpy(chForwardChannel,"879.390");
		bReverseChannel = true;
		chChannelBand[0]='A';
        bOK	= true;
	}



    ///////////// Band 'B' /////////////////////////
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.020")==0)  // FORWARD
	{
		strcpy(chChannel,"334");
        strcpy(chReverseChannel,"835.020");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.020")==0)//REVERSE
	{
		strcpy(chChannel,"334");
        strcpy(chForwardChannel,"880.020");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.050")==0)  // FORWARD
	{
		strcpy(chChannel,"335");
        strcpy(chReverseChannel,"835.050");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.050")==0)//REVERSE
	{
		strcpy(chChannel,"335");
        strcpy(chForwardChannel,"880.050");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}


	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.080")==0)  // FORWARD
	{
		strcpy(chChannel,"336");
        strcpy(chReverseChannel,"835.080");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.080")==0)//REVERSE
	{
		strcpy(chChannel,"336");
        strcpy(chForwardChannel,"880.080");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.110")==0)  // FORWARD
	{
		strcpy(chChannel,"337");
        strcpy(chReverseChannel,"835.110");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.110")==0)//REVERSE
	{
		strcpy(chChannel,"337");
        strcpy(chForwardChannel,"880.110");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.140")==0)  // FORWARD
	{
		strcpy(chChannel,"338");
        strcpy(chReverseChannel,"835.140");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.140")==0)//REVERSE
	{
		strcpy(chChannel,"338");
        strcpy(chForwardChannel,"880.140");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}
    
	
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.170")==0)  // FORWARD
	{
		strcpy(chChannel,"339");
        strcpy(chReverseChannel,"835.170");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.170")==0)//REVERSE
	{
		strcpy(chChannel,"339");
        strcpy(chForwardChannel,"880.170");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}
    
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.200")==0)  // FORWARD
	{
		strcpy(chChannel,"340");
        strcpy(chReverseChannel,"835.200");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.200")==0)//REVERSE
	{
		strcpy(chChannel,"340");
        strcpy(chForwardChannel,"880.200");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}
	
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.230")==0)  // FORWARD
	{
		strcpy(chChannel,"341");
        strcpy(chReverseChannel,"835.230");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.230")==0)//REVERSE
	{
		strcpy(chChannel,"341");
        strcpy(chForwardChannel,"880.230");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}
	
	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.260")==0)  // FORWARD
	{
		strcpy(chChannel,"342");
        strcpy(chReverseChannel,"835.260");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.260")==0)//REVERSE
	{
		strcpy(chChannel,"342");
        strcpy(chForwardChannel,"880.260");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}


	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.290")==0)  // FORWARD
	{
		strcpy(chChannel,"343");
        strcpy(chReverseChannel,"835.290");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.290")==0)//REVERSE
	{
		strcpy(chChannel,"343");
        strcpy(chForwardChannel,"880.290");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}
	

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.320")==0)  // FORWARD
	{
		strcpy(chChannel,"344");
        strcpy(chReverseChannel,"835.320");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.320")==0)//REVERSE
	{
		strcpy(chChannel,"344");
        strcpy(chForwardChannel,"880.320");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}
	

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.350")==0)  // FORWARD
	{
		strcpy(chChannel,"345");
        strcpy(chReverseChannel,"835.350");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.350")==0)//REVERSE
	{
		strcpy(chChannel,"345");
        strcpy(chForwardChannel,"880.350");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}


	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.380")==0)  // FORWARD
	{
		strcpy(chChannel,"346");
        strcpy(chReverseChannel,"835.380");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.380")==0)//REVERSE
	{
		strcpy(chChannel,"346");
        strcpy(chForwardChannel,"880.380");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.410")==0)  // FORWARD
	{
		strcpy(chChannel,"347");
        strcpy(chReverseChannel,"835.410");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.410")==0)//REVERSE
	{
		strcpy(chChannel,"347");
        strcpy(chForwardChannel,"880.410");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}


	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.440")==0)  // FORWARD
	{
		strcpy(chChannel,"348");
        strcpy(chReverseChannel,"835.440");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.440")==0)//REVERSE
	{
		strcpy(chChannel,"348");
        strcpy(chForwardChannel,"880.440");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}


	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.470")==0)  // FORWARD
	{
		strcpy(chChannel,"349");
        strcpy(chReverseChannel,"835.470");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.470")==0)//REVERSE
	{
		strcpy(chChannel,"349");
        strcpy(chForwardChannel,"880.470");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.500")==0)  // FORWARD
	{
		strcpy(chChannel,"350");
        strcpy(chReverseChannel,"835.500");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.500")==0)//REVERSE
	{
		strcpy(chChannel,"350");
        strcpy(chForwardChannel,"880.500");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.530")==0)  // FORWARD
	{
		strcpy(chChannel,"351");
        strcpy(chReverseChannel,"835.530");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.530")==0)//REVERSE
	{
		strcpy(chChannel,"351");
        strcpy(chForwardChannel,"880.530");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.560")==0)  // FORWARD
	{
		strcpy(chChannel,"352");
        strcpy(chReverseChannel,"835.560");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.560")==0)//REVERSE
	{
		strcpy(chChannel,"352");
        strcpy(chForwardChannel,"880.560");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.590")==0)  // FORWARD
	{
		strcpy(chChannel,"353");
        strcpy(chReverseChannel,"835.590");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.590")==0)//REVERSE
	{
		strcpy(chChannel,"353");
        strcpy(chForwardChannel,"880.590");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	////////////////////////////////////////////////
    if(strcmp(chFrequency,"880.620")==0)  // FORWARD
	{
		strcpy(chChannel,"354");
        strcpy(chReverseChannel,"835.620");
		bReverseChannel = false;
		chChannelBand[0]='B';
        bOK	= true;
	}

    if(strcmp(chFrequency,"835.620")==0)//REVERSE
	{
		strcpy(chChannel,"354");
        strcpy(chForwardChannel,"880.620");
		bReverseChannel = true;
		chChannelBand[0]='B';
        bOK	= true;
	}

	if(bOK != true)
	{
        MyError("This is not a valid Control Channel Frequency\nPlease choose a different one");
        return;
	}

	
	if(bReverseChannel == false)
    {
		m_ButtonSwitchControlType.SetWindowText("Switch To Reverse Control");
		strcpy(chChannelType,"Forward Control");
    }
	else
	{
		m_ButtonSwitchControlType.SetWindowText("Switch To Forward Control");
		strcpy(chChannelType,"Reverse Control");
	}
    
											 /* 
	if (iWhichcontrolChannel == 1)
	 {
		  stanum=0x80; endnum=0x50;
		  //strcpy(name,"TELCEL");
		  strcpy(chChannelType,"Forward Control");
		  strcpy(chGoFrequency,"880.500");
		  // strcpy(chChannel,"0350");
		  chChannelBand[0]='B';
	 }

	 if (iWhichcontrolChannel == 2)
	 {
		  //strcpy(name,"Baja Cellular");
		  strcpy(chChannelType,"Forward Control");
		  
		  // Most of the time this one
		   stanum=0x79; endnum=0x54;
		   strcpy(chGoFrequency,"879.540");
		   // strcpy(chChannel,"0318");

		  // But sometimes this one
          //stanum=0x79; endnum=0x42;
		  //strcpy(chGoFrequency,"879.420");
		  //strcpy(chChannel,"0314");

		  chChannelBand[0]='A';
	 }

	 if (iWhichcontrolChannel == 3)
	 {
		  stanum=0x35; endnum=0x50;
		  //strcpy(name,"TELCEL");
		  strcpy(chChannelType,"Reverse Control");
		  strcpy(chGoFrequency,"835.500");
		  // strcpy(chChannel,"0350");
		  chChannelBand[0]='B';
	 }

	if (iWhichcontrolChannel == 4)
	{
		 stanum=0x34; endnum=0x54;
		 //strcpy(name,"Baja Cellular");
		 strcpy(chChannelType,"Reverse Control");
		 strcpy(chGoFrequency,"834.540");
		 // strcpy(chChannel,"0318");

		  // But sometimes this one
		  // stanum=0x34; endnum=0x42;
		  // strcpy(chGoFrequency,"834.420");
		  // strcpy(chChannel,"0314");
		 
		 chChannelBand[0]='A';
	}
	*/

	UpdateChannelStats();
}


void CCELLULARDlg::InitalizeOS456(void)
{
    WriteToStatusBox(" Initalizing OS456");
 
	ComInit(iCommPortOS456);
    if(ComValid(iCommPortOS456))
    {
	    ComSetState(iCommPortOS456,9600,8,1,'N',SIGNAL_NONE);
   	    ComBufClear(iCommPortOS456,DIR_INC); 
	    ComBufClear(iCommPortOS456,DIR_OUT); 
		WriteToStatusBox(" OS456 Comm Port Initalized @ 9600 BPS");
    }
    else 
    {
	    char chTempString[250];
        char *pchTempString = chTempString;
	    sprintf(pchTempString,"Com Port %d is not available or is in use",iCommPortOS456+1);
	    MyError(pchTempString);
		return;
    }
  
    ComDTROn(iCommPortOS456);  // set DTR
    ComRTSOn(iCommPortOS456);  // set RTS

    osremote();
    osmode();
    osspeakeroff();
    osfreq(endnum,stanum);
}

// OS456 General access Commands
void CCELLULARDlg::os456(void)
{
    // Sleep(100);
    Sleep(30);

	ComWrite(iCommPortOS456,(char)0xFE);
    ComWrite(iCommPortOS456,(char)0xFE);
    ComWrite(iCommPortOS456,(char)0x80);
    ComWrite(iCommPortOS456,(char)0xE0);
}


// Switch OS456 transfer mode to FM-narrowband
void CCELLULARDlg::osmode(void)
{
	 os456();
	 ComWrite(iCommPortOS456,(char)0x01);
	 ComWrite(iCommPortOS456,(char)0x05);
	 ComWrite(iCommPortOS456,(char)0xFD);
}

// Switch OS456 to Local Control
void CCELLULARDlg::oslocal(void)
{
	 os456();
	 ComWrite(iCommPortOS456,(char)0x7F);
	 ComWrite(iCommPortOS456,(char)0x01);
	 ComWrite(iCommPortOS456,(char)0xFD);
}

// Switch OS456 to Remote Control
void CCELLULARDlg::osremote(void)
{
	 os456();
	 ComWrite(iCommPortOS456,(char)0x7F);
	 ComWrite(iCommPortOS456,(char)0x02);
	 ComWrite(iCommPortOS456,(char)0xFD);
}

// Switch OS456 Frequency
void CCELLULARDlg::osfreq(int endnum,int stanum)
{
	 int iTest = 1;
     
     again:
	 os456();
	 ComWrite(iCommPortOS456,(char)0x00);  // always
	 ComWrite(iCommPortOS456,(char)0x00);  // always
	 ComWrite(iCommPortOS456,(char)0x00);  // always
	 ComWrite(iCommPortOS456,(char)endnum);  // 880.XX00 0x50
	 ComWrite(iCommPortOS456,(char)stanum);  // 8XX.5000 0x80
	 ComWrite(iCommPortOS456,(char)0x08);  // X80.5000 (always 8 for 800 band cellular)
	 ComWrite(iCommPortOS456,(char)0xFD);  // always
	 if(iTest==5)
	 {
	     WriteToStatusBox(" Error : in fuction [osfreqcheck] (5 Tries!)");
         return;		  
	 }
	 
	 iTest ++;
	 if (osfreqcheck()== 1){ goto again;}
     
 	 // signal strength
	 // Sleep(100);
	 ossignalstr();

	 UpdateChannelStats();
}


int CCELLULARDlg::osfreqcheck(void)
{
/*
    Sleep(250); 
	ComBufClear(iCommPortOS456,DIR_INC); 
    osreadfreq();
    Sleep(250);

    char ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9,ch10,ch11,ch12,ch13,ch14,ch15,ch16;
	
	if(ComRead(iCommPortOS456,&ch1)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 1");
    }
	if(ComRead(iCommPortOS456,&ch2)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 2");
    }
	if(ComRead(iCommPortOS456,&ch3)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 3");
    }
	if(ComRead(iCommPortOS456,&ch4)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 4");
    }
	if(ComRead(iCommPortOS456,&ch5)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 5");
    }
	if(ComRead(iCommPortOS456,&ch6)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 6");
    }
	if(ComRead(iCommPortOS456,&ch7)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 7");
    }
	if(ComRead(iCommPortOS456,&ch8)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 8");
    }
	if(ComRead(iCommPortOS456,&ch9)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 9");
    }
	if(ComRead(iCommPortOS456,&ch10)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 10");
    }
	if(ComRead(iCommPortOS456,&ch11)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 11");
    }
	if(ComRead(iCommPortOS456,&ch12)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 12");
    }
	if(ComRead(iCommPortOS456,&ch13)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 13");
    }
	if(ComRead(iCommPortOS456,&ch14)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 14");
    }
	if(ComRead(iCommPortOS456,&ch15)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 15");
    }
	if(ComRead(iCommPortOS456,&ch16)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 Frequency reading byte 16");
    }

    char chTempString[500];
	char *pchTempString = chTempString;
	TRACE("FREQ DATA: 1[%2X] 2[%2X] 3[%2X] 4[%2X] 5[%2X] 6[%2X] 7[%2X] 8[%2X] 9[%2X] 10[%2X] 11[%2X] 12[%2X] 13[%2X] 14[%2X] 15[%2X] 16[%2X]\n",
			            ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9,ch10,ch11,ch12,ch13,ch14,ch15,ch16);

	TRACE("FREQ2 DATA: endnum[%2X] stanum[%2X] \n",endnum,stanum);


    if ((ch12!=0x00)|| (ch13!=0x00) || (ch14!=endnum) || (ch15!=stanum) || (ch16!=0x08)) 
	//&& ((ch14!=0x00)|| (ch15!=0x00) || (ch16!=endnum) || (ch17!=stanum) || (ch18!=0x08))
    // && ((ch15!=0x00)|| (ch16!=0x00) || (ch17!=endnum) || (ch18!=stanum) || (ch19!=0x08)) )
    {
	       WriteToStatusBox(" ERROR: OS456 Frequency Check Failed so trying again");
    	   TRACE("FREQ ERROR\n");
		   // char chTempString[250];
		   //char *pchTempString = chTempString;
	 	   //sprintf(pchTempString," [%c][%c][%c]",ch17,ch18,ch19);
		   //WriteToStatusBox(pchTempString);
		   return 1;
   }
*/
  return OK;
}

void CCELLULARDlg::osreadfreq(void)
{
	 os456();
	 ComWrite(iCommPortOS456,(char)0x03);
	 ComWrite(iCommPortOS456,(char)0xFD);
}


// Switch OS456 Speaker OFF
void CCELLULARDlg::osspeakeroff(void)
{
    int iTest=1;
  
    again:
   
    ComBufClear(iCommPortOS456,DIR_INC); 
    os456();
	ComWrite(iCommPortOS456,(char)0x7F);
	ComWrite(iCommPortOS456,(char)0x0B);
	ComWrite(iCommPortOS456,(char)0xFD);
	iTest++;
	if(iTest==5)
	{
	     MyError("Error : in fuction [osspeakercheck (osspeakeroff)] (5 Tries!)");
	}
    iTest++;

    if(osspeakercheck()== 1){ goto again;}
}

// Switch OS456 Speaker ON
void CCELLULARDlg::osspeakeron(void)
{
    int iTest=1;
  
    again:
   	
	ComBufClear(iCommPortOS456,DIR_INC); 
	os456();
	ComWrite(iCommPortOS456,(char)0x7F);
	ComWrite(iCommPortOS456,(char)0x0A);
	ComWrite(iCommPortOS456,(char)0xFD);
	iTest++;
	if(iTest==5)
	{
	     MyError("Error : in fuction [osspeakercheck (osspeakeron)] (5 Tries!)");
	 }
	 iTest++;
	 if (osspeakercheck()== 1){ goto again;}
 }

int CCELLULARDlg::osspeakercheck(void)
{
    char ch1=0,ch2=0,ch3=0,ch4=0,ch5=0,ch6=0,ch7=0,ch8=0,ch9=0,ch10=0,ch11=0,ch12=0,ch13=0;
    Sleep(250);
    // Sleep(30);
	// TRYAGAIN:
    // if(ComBufCount(iCommPortOS456,DIR_INC) >= 15) 
	{                                              
	     if(ComRead(iCommPortOS456,&ch1)==FALSE)
		 {
	         WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 1");
		 }
	     if(ComRead(iCommPortOS456,&ch2)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 2");
		 }
	     if(ComRead(iCommPortOS456,&ch3)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 3");
		 }
	     if(ComRead(iCommPortOS456,&ch4)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 4");
		 }
	     if(ComRead(iCommPortOS456,&ch5)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 5");
		 }
	     if(ComRead(iCommPortOS456,&ch6)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 6");
		 }
	     if(ComRead(iCommPortOS456,&ch7)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 7");
		 }
	     if(ComRead(iCommPortOS456,&ch8)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 8");
		 }
	     if(ComRead(iCommPortOS456,&ch9)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 9");
		 }
	     if(ComRead(iCommPortOS456,&ch10)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 10");
		 }
	     if(ComRead(iCommPortOS456,&ch11)==FALSE)
		 {
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 11");
		 }
	    if(ComRead(iCommPortOS456,&ch12)==FALSE)
		{
     	    WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 12");
		}
	    if(ComRead(iCommPortOS456,&ch13)==FALSE)
		{
	        WriteToStatusBox(" ERROR: OS456 osspeakercheck reading byte 13");
		}
    }
/*
	else
	{
	    Sleep(100);

        char chTempString[100];
		char *pchTempString = chTempString;
		sprintf(pchTempString," OS456 Buffercount = %d ",ComBufCount(iCommPortOS456,DIR_INC) );
		WriteToStatusBox(pchTempString);

		goto TRYAGAIN;
	}
*/
        char chTempString[500];
		char *pchTempString = chTempString;
		TRACE("SPEAKER DATA:1[%2X] 2[%2X] 3[%2X] 4[%2X] 5[%2X] 6[%2X] 7[%2X] 8[%2X] 9[%2X] 10[%2X] 11[%2X] 12[%2X] 13[%2X]\n",
			            ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9,ch10,ch11,ch12,ch13);

     if((ch10!=0xFFFFFFE0)|| (ch11!=0xFFFFFF80) || (ch12!=0xFFFFFFFB) || (ch13!=0xFFFFFFFD))
 	 // if(((ch10!=0xE0)|| (ch11!=0x80) || (ch12!=0xFB))
     // &&((ch12!=0xE0)|| (ch13!=0x80) || (ch14!=0xFB))
	 // &&((ch13!=0xE0)|| (ch14!=0x80) || (ch15!=0xFB)))
	 {
	     WriteToStatusBox(" ERROR: OS456 Speaker Check Failed!");
		 return 1;
	 }
     else
	 {
	     // WriteToStatusBox(" OS456 Speaker Check Returned OK");
		 return OK;
     }
}
void CCELLULARDlg::ossignalstr(void)
{
	 ComBufClear(iCommPortOS456,DIR_INC); 
	 os456();
	 ComWrite(iCommPortOS456,(char)0x15);
	 ComWrite(iCommPortOS456,(char)0x02);
	 ComWrite(iCommPortOS456,(char)0xFD);

	 Sleep(30);

    char ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9,ch10,ch11,ch12,ch13,ch14,ch15;
	
	if(ComRead(iCommPortOS456,&ch1)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 1");
    }
	if(ComRead(iCommPortOS456,&ch2)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 2");
    }
	if(ComRead(iCommPortOS456,&ch3)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 3");
    }
	if(ComRead(iCommPortOS456,&ch4)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 4");
    }
	if(ComRead(iCommPortOS456,&ch5)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 5");
    }
	if(ComRead(iCommPortOS456,&ch6)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 6");
    }
	if(ComRead(iCommPortOS456,&ch7)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 7");
    }
	if(ComRead(iCommPortOS456,&ch8)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 8");
    }
	if(ComRead(iCommPortOS456,&ch9)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 9");
    }
	if(ComRead(iCommPortOS456,&ch10)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 10");
    }
	if(ComRead(iCommPortOS456,&ch11)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 11");
    }
	if(ComRead(iCommPortOS456,&ch12)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 12");
    }
	if(ComRead(iCommPortOS456,&ch13)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 13");
    }
	if(ComRead(iCommPortOS456,&ch14)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 14");
    }
	if(ComRead(iCommPortOS456,&ch15)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossignalstr reading byte 15");
    }

	 // sprintf(chSignalStrength,"-%X.%X dBm",ch14,ch15);

	 if(ch14 == 0x00)
	 {
         if(ch15 == 0x00)
		 {
             sprintf(chSignalStrength,"- 0 dBm",ch15);
		 }
         else if(ch15 == 0x01)
		 {
             sprintf(chSignalStrength,"- 1 dBm",ch15);
		 }
         else if(ch15 == 0x02)
		 {
             sprintf(chSignalStrength,"- 2 dBm",ch15);
		 }
         else if(ch15 == 0x03)
		 {
             sprintf(chSignalStrength,"- 3 dBm",ch15);
		 }
         else if(ch15 == 0x04)
		 {
             sprintf(chSignalStrength,"- 4 dBm",ch15);
		 }
         else if(ch15 == 0x05)
		 {
             sprintf(chSignalStrength,"- 5 dBm",ch15);
		 }
         else if(ch15 == 0x06)
		 {
             sprintf(chSignalStrength,"- 6 dBm",ch15);
		 }
         else if(ch15 == 0x07)
		 {
             sprintf(chSignalStrength,"- 7 dBm",ch15);
		 }
         else if(ch15 == 0x08)
		 {
             sprintf(chSignalStrength,"- 8 dBm",ch15);
		 }
         else if(ch15 == 0x09)
		 {
             sprintf(chSignalStrength,"- 9 dBm",ch15);
		 }
         else if(ch15 >= 0x10)
		 {
             sprintf(chSignalStrength,"- %X dBm",ch15);	 
		 }
	 }
	 
	 if(ch14 == 0x01)
	 {
         if(ch15 == 0x00)
		 {
             sprintf(chSignalStrength,"- 100 dBm",ch15);
		 }
         else if(ch15 == 0x01)
		 {
             sprintf(chSignalStrength,"- 101 dBm",ch15);
		 }
         else if(ch15 == 0x02)
		 {
             sprintf(chSignalStrength,"- 102 dBm",ch15);
		 }
         else if(ch15 == 0x03)
		 {
             sprintf(chSignalStrength,"- 103 dBm",ch15);
		 }
         else if(ch15 == 0x04)
		 {
             sprintf(chSignalStrength,"- 104 dBm",ch15);
		 }
         else if(ch15 == 0x05)
		 {
             sprintf(chSignalStrength,"- 105 dBm",ch15);
		 }
         else if(ch15 == 0x06)
		 {
             sprintf(chSignalStrength,"- 106 dBm",ch15);
		 }
         else if(ch15 == 0x07)
		 {
             sprintf(chSignalStrength,"- 107 dBm",ch15);
		 }
         else if(ch15 == 0x08)
		 {
             sprintf(chSignalStrength,"- 108 dBm",ch15);
		 }
         else if(ch15 == 0x09)
		 {
             sprintf(chSignalStrength,"- 109 dBm",ch15);
		 }
         else if(ch15 >= 0x10)
		 {
             sprintf(chSignalStrength,"- 1%X dBm",ch15);	 
		 }
	 }

     m_EditSignalStrength.SetWindowText(chSignalStrength);

     
	 char chTempString[100];
	 char *pchTempString = chTempString;
	 sprintf(pchTempString,"%X%X",ch14,ch15);

     int iSignal = atoi(pchTempString);

	 if(iSignal <= 125)
	 {	 
	     if(iSignal <= 100)
		 {
	         m_ProgressSignal.SetPos(25);
         }
 	     else if(iSignal == 101)
		 {
	         m_ProgressSignal.SetPos(24);
         }
 	     else if(iSignal == 102)
		 {
	         m_ProgressSignal.SetPos(23);
         }
 	     else if(iSignal == 103)
		 {
	         m_ProgressSignal.SetPos(22);
         }
 	     else if(iSignal == 104)
		 {
	         m_ProgressSignal.SetPos(21);
         }
 	     else if(iSignal == 105)
		 {
	         m_ProgressSignal.SetPos(20);
         }
 	     else if(iSignal == 106)
		 {
	         m_ProgressSignal.SetPos(19);
         }
 	     else if(iSignal == 107)
		 {
	         m_ProgressSignal.SetPos(18);
         }
 	     else if(iSignal == 108)
		 {
	         m_ProgressSignal.SetPos(17);
         }
 	     else if(iSignal == 109)
		 {
	         m_ProgressSignal.SetPos(16);
         }
 	     else if(iSignal == 110)
		 {
	         m_ProgressSignal.SetPos(15);
         }
 	     else if(iSignal == 111)
		 {
	         m_ProgressSignal.SetPos(14);
         }
 	     else if(iSignal == 112)
		 {
	         m_ProgressSignal.SetPos(13);
         }
 	     else if(iSignal == 113)
		 {
	         m_ProgressSignal.SetPos(12);
         }
 	     else if(iSignal == 114)
		 {
	         m_ProgressSignal.SetPos(11);
         }
 	     else if(iSignal == 115)
		 {
	         m_ProgressSignal.SetPos(10);
         }
 	     else if(iSignal == 116)
		 {
	         m_ProgressSignal.SetPos(9);
         }
 	     else if(iSignal == 117)
		 {
	         m_ProgressSignal.SetPos(8);
         }
 	     else if(iSignal == 118)
		 {
	         m_ProgressSignal.SetPos(7);
         }
 	     else if(iSignal == 119)
		 {
	         m_ProgressSignal.SetPos(6);
         }
 	     else if(iSignal == 120)
		 {
	         m_ProgressSignal.SetPos(5);
         }
 	     else if(iSignal == 121)
		 {
	         m_ProgressSignal.SetPos(4);
         }
 	     else if(iSignal == 122)
		 {
	         m_ProgressSignal.SetPos(3);
         }
 	     else if(iSignal == 123)
		 {
	         m_ProgressSignal.SetPos(2);
         }
 	     else if(iSignal == 124)
		 {
	         m_ProgressSignal.SetPos(1);
         }
 	     else if(iSignal == 125)
		 {
	         m_ProgressSignal.SetPos(0);
         }
     }
	 
}

void CCELLULARDlg::ossquelch(void)
{
     ComBufClear(iCommPortOS456,DIR_INC); 
	 os456();

	 ComWrite(iCommPortOS456,(char)0x15);
	 ComWrite(iCommPortOS456,(char)0x01);
	 ComWrite(iCommPortOS456,(char)0xFD);

     Sleep(250);
     // Sleep(30);


    char ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9,ch10,ch11,ch12,ch13,ch14;
	
	if(ComRead(iCommPortOS456,&ch1)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 1");
    }
	if(ComRead(iCommPortOS456,&ch2)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 2");
    }
	if(ComRead(iCommPortOS456,&ch3)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 3");
    }
	if(ComRead(iCommPortOS456,&ch4)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 4");
    }
	if(ComRead(iCommPortOS456,&ch5)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 5");
    }
	if(ComRead(iCommPortOS456,&ch6)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 6");
    }
	if(ComRead(iCommPortOS456,&ch7)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 7");
    }
	if(ComRead(iCommPortOS456,&ch8)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 8");
    }
	if(ComRead(iCommPortOS456,&ch9)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 9");
    }
	if(ComRead(iCommPortOS456,&ch10)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 10");
    }
	if(ComRead(iCommPortOS456,&ch11)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 11");
    }
	if(ComRead(iCommPortOS456,&ch12)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 12");
    }
	if(ComRead(iCommPortOS456,&ch13)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 13");
    }
	if(ComRead(iCommPortOS456,&ch14)==FALSE)
	{
	    WriteToStatusBox(" ERROR: OS456 ossquelch reading byte 14");
    }

    char chTempString[500];
	char *pchTempString = chTempString;
	TRACE("SQUELCH DATA: 1[%2X] 2[%2X] 3[%2X] 4[%2X] 5[%2X] 6[%2X] 7[%2X] 8[%2X] 9[%2X] 10[%2X] 11[%2X] 12[%2X] 13[%2X] 14[%2X]\n",
			            ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9,ch10,ch11,ch12,ch13,ch14);


	 if (ch14==0x00)
	 {
		 WriteToStatusBox(" OS456 reports Squelch is CLOSED");
		 squelch = CLOSED;
	 }
	 if (ch14==0x01)
	 {
	     TRACE("OS456 reports Squelch is OPEN");
		 squelch = OPEN;
	 }
}



void CCELLULARDlg::SetDDIvoiceMode(void)
{
    WriteToStatusBox(" Sending DDI Command to switch to VOICE mode.");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'V'); 
}


void CCELLULARDlg::SetDDItoControlChannel(void)
{
    WriteToStatusBox(" Sending DDI Command to switch to Control Channel");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'C'); 
}

void CCELLULARDlg::SetDDItoForward(void)
{
    WriteToStatusBox(" Sending DDI Command to switch to Forward Channel Mode");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'r'); 

	bReverseChannel = false;
}

void CCELLULARDlg::SetDDItoReverse(void)
{
    WriteToStatusBox(" Sending DDI Command to switch to Reverse Channel Mode");
	ComWrite(iCommPortDDI,'-');
	ComWrite(iCommPortDDI,'R'); 

	bReverseChannel = true;
}


void CCELLULARDlg::check_call_time(void)
{
    time_t current_time;
    current_time=time(NULL);
    if(current_time>=start_time+iMaxCallTime)
	{
	    squelch=CLOSED;
	}
}

void CCELLULARDlg::switch_to_voice(void)
{
    MessageBeep(MB_OK);
  
	
			/*
			if(logit==TRUE)
			{
				fprintf(log," \n");
			}
			*/

            ConvertGoFrequency();

			SetDDIvoiceMode();

			strcpy(chChannelType,"Forward Voice");

			osfreq(endnum,stanum);
            ComBufClear(iCommPortDDI,DIR_INC); 

			osspeakeron();

            /*
			for(j=0;j < BUFFERSIZE2 ;j++)
				{
				 for(k=0;k < BUFFERLENGTH ;k++)
					 {voicebuffer[j][k] =(char) 0x00;}
				}
			whichbuffer2=1;
			noofrings=0;
			ring=FALSE;
            */


			// call started
			m_EditStart.SetWindowText(timeis);
            m_EditCallEnd.SetWindowText(" ");

			start_time=time(NULL); // Start Second Timer for iMaxCallTime Function

			if(phonelst.Darea[0]==0x00)
			{
				strcpy(phonelst.Darea,chArea);
			}
			if(phonelst.Dtype[0]==0x00)
			{
				phonelst.Dtype[0]=chChannelBand[0];
			}
			if(phonelst.DLastCallDate[0]==0x00)
			{
				strcpy(phonelst.DLastCallDate,dateis); 
			}
			if(phonelst.DLastCallTime[0]==0x00)
			{
				strcpy(phonelst.DLastCallTime,d_timeis); 
			}
			
			if(phonelst.Dphone[0]==0x00)
			{
				 strcpy(phonelst.Dphone,chPhoneNumber);
				 phonelst.Dphone[14]=0x00;
				 insert_record();
			}
			show_database();
			
			// Increment Total Call Counter
    		char chTempString[50];
			char *pchTempString = chTempString;
			
			iCallCounter_total++;
			sprintf(pchTempString,"%d",iCallCounter_total);
		    m_Text_CallsTotal.SetWindowText(pchTempString);
			
			if(phonelst.Dalert[0]==0x00||phonelst.Dalert[0]=='0')
			{
				iCallCounter_0++;
				sprintf(pchTempString,"%d",iCallCounter_0);
				m_Text_CallsLevel0.SetWindowText(pchTempString);
			}
			
			if(phonelst.Dalert[0]=='1')
			{
				iCallCounter_1++;
				sprintf(pchTempString,"%d",iCallCounter_1);
				m_Text_CallsLevel1.SetWindowText(pchTempString);
			}
			
			if(phonelst.Dalert[0]=='2')
			{
				iCallCounter_2++;
				sprintf(pchTempString,"%d",iCallCounter_2);
				m_Text_CallsLevel2.SetWindowText(pchTempString);
			}

			if(phonelst.Dalert[0]=='3')
			{
				iCallCounter_3++;
				sprintf(pchTempString,"%d",iCallCounter_3);
				m_Text_CallsLevel3.SetWindowText(pchTempString);
			}

			if(phonelst.Dalert[0]=='4')
			{
				iCallCounter_4++;
				sprintf(pchTempString,"%d",iCallCounter_4);
				m_Text_CallsLevel4.SetWindowText(pchTempString);
			}

			if(phonelst.Dalert[0]=='5')
			{
				iCallCounter_5++;
				sprintf(pchTempString,"%d",iCallCounter_5);
				m_Text_CallsLevel5.SetWindowText(pchTempString);
			}

			if(phonelst.Dalert[0]=='6')
			{
				iCallCounter_6++;
				sprintf(pchTempString,"%d",iCallCounter_6);
				m_Text_CallsLevel6.SetWindowText(pchTempString);
			}

			if(phonelst.Dalert[0]=='7')
			{
				iCallCounter_7++;
				sprintf(pchTempString,"%d",iCallCounter_7);
				m_Text_CallsLevel7.SetWindowText(pchTempString);
			}

			if(phonelst.Dalert[0]=='8')
			{
				iCallCounter_8++;
				sprintf(pchTempString,"%d",iCallCounter_8);
				m_Text_CallsLevel8.SetWindowText(pchTempString);
			}

			if(phonelst.Dalert[0]=='9')
			{
				iCallCounter_9++;
				sprintf(pchTempString,"%d",iCallCounter_9);
				m_Text_CallsLevel9.SetWindowText(pchTempString);
			}

			if(phonelst.Dmodem[0]=='Y')
			{
				iCallCounter_modem++;
				sprintf(pchTempString,"%d",iCallCounter_modem);
			    m_Text_CallsModem.SetWindowText(pchTempString);
			}
			
			if(phonelst.Dfax[0]=='Y')
			{
				iCallCounter_fax++;
				sprintf(pchTempString,"%d",iCallCounter_fax);
			    m_Text_CallsFax.SetWindowText(pchTempString);
			}
            
		
            /*
			if(logit==TRUE)
			{
				fprintf(log,"\n %s %s (%s)\n"
				,timeis,number,area);
				fprintf(log,"            Changing to Voice Channel %s at %s MHz\n"
				,chChannel,chGoFrequency);
				fprintf(log,"            %s\n",chSignalStrength);
			}
            */

			bCancelCall = false;
            


			// rev = FALSE;
			
			/*
			if(((phonelst.DRecordCall[0]==Y)||(bRecordAll==TRUE))&&(bRecording==FALSE))
			  {
				rec_raw();
				bRecording = TRUE;
				record_on_box();

				iCallCounter_record++;
				sprintf(pchTempString,"%04d",iCallCounter_record);
				m_Text_CallsRecorded.SetWindowText(pchTempString);
			  }
            */

            Sleep(250); // This is to give the MCI Device time to respond

            ////////////////////////////////////////////////////////////////////////
            
            #ifdef RECORD
			DWORD dwReturn;
            MCI_OPEN_PARMS mciOpenParms;
            MCI_RECORD_PARMS mciRecordParms;

            // Open a waveform-audio device with a new file for recording.
            mciOpenParms.lpstrDeviceType = "waveaudio";
            mciOpenParms.lpstrElementName = "";
            if(dwReturn = mciSendCommand(0, MCI_OPEN,MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,(DWORD)(LPVOID) &mciOpenParms)) 
			{
                MyError("Failed to open MCI device;");
                return;
			}

            // The device opened successfully; get the device ID.
            wDeviceID = mciOpenParms.wDeviceID;

            // Begin recording and record for the specified number of 
            // milliseconds. Wait for recording to complete before continuing. 
            // Assume the default time format for the waveform-audio device 
            // (milliseconds).
            
			//mciRecordParms.dwTo = dwMilliSeconds;
            
			if(dwReturn = mciSendCommand(wDeviceID, MCI_RECORD,MCI_TO , (DWORD)(LPVOID) &mciRecordParms))
			{
                 mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
				 MyError("Failed to to tell MCI device to record.");
                 return;
			}
            ////////////////////////////////////////////////////////////////////////
            #endif 

            char chTotalTime[30];
            char *pchTotalTime = chTotalTime;
            strcpy(pchTotalTime,"NONE");

	
    	    char chTempBuffer[500];
     	    char *pchTempBuffer = chTempBuffer;
			sprintf(pchTempBuffer," %s %s\tGoto Voice Chann\t%s",timeis,chPhoneNumber,chArea);
			WriteToStatusBox(pchTempBuffer);

			ossquelch();

		    for(int i=0; i < 5; i++)
			{
		        if(squelch==CLOSED)
				{
                     Sleep(100);
				     ossquelch();
				}
			    else
				{
				    break;
				}
            }


			while(squelch==OPEN)
			{
		  	    ossignalstr();

				if(bGoReverseVoice == true)
				{
                    bGoReverseVoice = false;
					GoReverseVoice();
                }
				
				if(bGoForwardVoice == true)
				{
                    bGoForwardVoice = false;
					GoForwardVoice();
                }

				if(bHoldCall==false)
				{
            	    ossquelch();
				    
					check_call_time();
                }
			
				if(bCancelCall==true)
				{
 			        bCancelCall=false;
					WriteToStatusBox(" GOT SKIPPED VOICE COMMAND FROM MAIN FORWARD VOICE");

					break;
				}

				time_t current_time;
                current_time=time(NULL);
                double fSeconds = difftime(current_time,start_time);

  				if(fSeconds < 60)
				{
                    sprintf(pchTotalTime,"%.0f Seconds",fSeconds);
                }
                else
				{
                    double fMinutes =  fSeconds / 60;
					sprintf(pchTotalTime,"%.2f Minutes",fMinutes);
				}
                m_EditCallTime.SetWindowText(pchTotalTime);

                char i;
	            if(ComRead(iCommPortDDI,&i)==TRUE)
				{
		            if(i==0x0D)
					{
	                   char chTempBuffer[500];
			           char *pchTempBuffer = chTempBuffer;
			           iBufferCounter=-1;
    		           whattime();
			           AnalyzeDDIdata();

       			       if(bGoto ==true)
					   {
                            ZeroMemVariables();
					   }
                       else
		               if(bVoiceGoto ==true)
					   {
   				            sprintf(pchTempBuffer," %s %s\tGoto in Voice Function %s\t%s",timeis,chPhoneNumber,chGoFrequency,chArea);
				            WriteToStatusBox(pchTempBuffer);
                            ZeroMemVariables();
							
							ConvertGoFrequency();
							squelch=OPEN;
							osfreq(endnum,stanum);
                            Sleep(100);
							squelch=OPEN;
					   } 
					}	  // End if (i>1)
	                if((i>=0x20) && (i <=0x7A))
					{
	                  iBufferCounter++;
	                  chDDIdataBuffer[iBufferCounter] = (char) i;;
					}
				}

			} // End While

    		m_ButtonSwitchVoice.SetWindowText("Reverse Voice");
            bVoiceForward = true;

			// call ended
			whattime();
			m_EditCallEnd.SetWindowText(timeis);
			
			strcpy(phonelst.DLastCallDate,dateis);
			strcpy(phonelst.DLastCallTime,d_timeis);
			phonelst.DTotalCalls++;

            
			////////////////////////////////////////////////////////////////////////
			
			iWaveFileNumber++;
            WriteWhoFile(iWaveFileNumber,pchTotalTime);

			UpdateCallList(iWaveFileNumber,pchTotalTime);

			
            #ifdef RECORD
			sprintf(chTempBuffer,"RECORD\\%d.WAV",iWaveFileNumber);

			MCI_SAVE_PARMS mciSaveParms;
			mciSaveParms.lpfilename = chTempBuffer;

			// mciSaveParms.lpfilename = "tempfile.wav";
            if(dwReturn = mciSendCommand(wDeviceID, MCI_SAVE,MCI_SAVE_FILE | MCI_WAIT, (DWORD)(LPVOID) &mciSaveParms))
			{
                 mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
                 MyError("Failed to SAVE MCI device wavefile;");
				 return;
			}
            ////////////////////////////////////////////////////////////////////////
            mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
            #endif

			CopyEditFieldsToDataBase();
			
			strcpy(chWhereErrorOccured,"switch_to_voice");
			
			
			amend_record(); // Error Because of this Amend Routine

			ComBufClear(iCommPortDDI,DIR_INC);  // Flush DDI Recieve Port so extra stuff doesnt appear when finished

			ZeroMemVariables();
			osspeakeroff();

			SetDDItoControlChannel();
            SetDDItoForward();

			which_channel();
			osfreq(endnum,stanum);

            // UpdateChannelStats();
			/*
			if(logit==TRUE)
			{
				fprintf(log,"\n %s %s @ %s [%s]\n",name,type,chChannel,chGoFrequency);
				fprintf(log," %s\n\n",chSignalStrength);
			}
			*/

}
/*
    m_EditPhone.LimitText(14);
	m_EditName.LimitText(30);
    m_EditName2.LimitText(30);
    m_EditArea.LimitText(30);
    m_EditOccupation.LimitText(30);
	m_EditSubject.LimitText(30);
    m_EditLanguage.LimitText(10);
    m_EditGender.LimitText(1);
    m_EditInfo1.LimitText(30);
    m_EditInfo2.LimitText(30);
    m_EditInfo3.LimitText(30);
    m_EditLinkedFile.LimitText(12);
    m_EditModemCalls.LimitText(1);
    m_EditFaxCalls.LimitText(1);
    m_EditAlertLevel.LimitText(1);
	m_EditRecordCall.LimitText(1);
    m_EditDtmf.LimitText(30);
    m_EditESN.LimitText(8);
	m_EditSCM.LimitText(2);

*/

void CCELLULARDlg::OnButtonSkipvoicecall() 
{
    WriteToStatusBox(" Skip voice call button pressed");

    bCancelCall=true;	
}

void CCELLULARDlg::CopyEditFieldsToDataBase(void)
{
    m_EditPhone.GetWindowText(phonelst.Dphone,15);
	m_EditName.GetWindowText(phonelst.Dname,31);
    m_EditName2.GetWindowText(phonelst.Dname2,31);
    m_EditArea.GetWindowText(phonelst.Darea,31);
    m_EditOccupation.GetWindowText(phonelst.Doccupation,31);
	m_EditSubject.GetWindowText(phonelst.Dsubject,31);
    m_EditLanguage.GetWindowText(phonelst.Dlanguage,11);
    m_EditGender.GetWindowText(phonelst.Dgender,2);
    m_EditInfo1.GetWindowText(phonelst.Dinfo1,31);
    m_EditInfo2.GetWindowText(phonelst.Dinfo2,31);
    m_EditInfo3.GetWindowText(phonelst.Dinfo3,31);
    m_EditLinkedFile.GetWindowText(phonelst.DLinkedFile,13);
    m_EditModemCalls.GetWindowText(phonelst.Dmodem,2);
    m_EditFaxCalls.GetWindowText(phonelst.Dfax,2);
    m_EditAlertLevel.GetWindowText(phonelst.Dalert,2);
	m_EditRecordCall.GetWindowText(phonelst.DRecordCall,2);
    m_EditDtmf.GetWindowText(phonelst.DDtmfDigits,31);
    m_EditESN.GetWindowText(phonelst.Desn,9);
	m_EditSCM.GetWindowText(phonelst.Dscm,3);
}

void CCELLULARDlg::OnButtonSaveData() 
{
    phonelst.clear_buf();
    m_EditPhone.GetWindowText(phonelst.Dphone,15);

    if((phonelst.find()== IM_OK))
	{
     	CopyEditFieldsToDataBase();
	    strcpy(chWhereErrorOccured,"OnButtonSaveData");
	    amend_record();
    }
	else
	{
		MyError("Could not append record in fuction OnButtonSaveData");
    }
}

void CCELLULARDlg::OnButtonFindPhone() 
{
    phonelst.clear_buf();
    m_EditPhone.GetWindowText(phonelst.Dphone,15);

    search();

	if((bNewNumber == true)||(bAmend==false))
	{
		 phonelst.clear_buf();
		 return;
	}
}





void CCELLULARDlg::OnCheckHoldcall() 
{
	if(bHoldCall==false)
	{
        bHoldCall = true;
		return;
	}
    else
	{
        bHoldCall = false;
	    return;
	}
}


void CCELLULARDlg::UpdateCallList(int iWaveFileNumber,char *pchTotalTime)
{
    char chTempBuffer[500];
    char *pchTempBuffer = chTempBuffer;

    if(phonelst.Dalert[0]==0x00)
    {
		phonelst.Dalert[0] = '0';
	}

    if(phonelst.Dname[0]==0x00)
	{
		sprintf(pchTempBuffer," %04d %s [%s] [%s] [%s] %s %s %s",iWaveFileNumber,timeis,chPhoneNumber,phonelst.Dalert,pchTotalTime,phonelst.Doccupation,phonelst.Dsubject,phonelst.Darea);
	}
    else
	{
        sprintf(pchTempBuffer," %04d %s [%s] [%s] [%s] (%s) %s %s",iWaveFileNumber,timeis, chPhoneNumber,phonelst.Dalert,pchTotalTime,phonelst.Dname,phonelst.Doccupation,phonelst.Dsubject);
	}

	WriteToCallBox(pchTempBuffer);
}


void CCELLULARDlg::WriteWhoFile(int iFilenum,char *pchTotalTime)
{
    char chTempFileName[100];
	char *pchTempFileName = chTempFileName;

	sprintf(chTempFileName,"RECORD\\%d.TXT",iFilenum);
    
	FILE *whofile;
	if((whofile=fopen(chTempFileName,"wt"))==0)
	{
         MyError("Opening Whofile for write");
	}
	else
	{
        fprintf(whofile,"\n");
        fprintf(whofile,"TIME       : %s %s\n",dateis,timeis);
        fprintf(whofile,"CALL TIME  : %s\n",pchTotalTime);
		fprintf(whofile,"NUMBER     : %s\n",phonelst.Dphone);
        fprintf(whofile,"AREA       : %s\n",phonelst.Darea);
        fprintf(whofile,"NAME       : %s\n",phonelst.Dname);
        fprintf(whofile,"NAME #2    : %s\n",phonelst.Dname2);
        fprintf(whofile,"OCCUPATION : %s\n",phonelst.Doccupation);
        fprintf(whofile,"SUBJECT    : %s\n",phonelst.Dsubject);
        fprintf(whofile,"GENDER     : %s\n",phonelst.Dgender);
        fprintf(whofile,"LANGUAGE   : %s\n",phonelst.Dlanguage);
        fprintf(whofile,"DTMF DIGITS: %s\n",phonelst.DDtmfDigits);
        fprintf(whofile,"INFO #1    : %s\n",phonelst.Dinfo1);
        fprintf(whofile,"INFO #2    : %s\n",phonelst.Dinfo2);
        fprintf(whofile,"INFO #3    : %s\n",phonelst.Dinfo3);
        fprintf(whofile,"LINKED FILE: %s\n",phonelst.DLinkedFile);
        fprintf(whofile,"ESN        : %s\n",phonelst.Desn);
        fprintf(whofile,"SCM        : %s\n",phonelst.Dscm);
        fprintf(whofile,"LAST CALL  : %s %s\n",phonelst.DLastCallDate,phonelst.DLastCallTime);
        fprintf(whofile,"TOTAL CALLS: %d\n",phonelst.DTotalCalls);
        fprintf(whofile,"MODEM      : %s\n",phonelst.Dmodem);
        fprintf(whofile,"FAX        : %s\n",phonelst.Dfax);
        fprintf(whofile,"ALERT LEVEL: %s\n",phonelst.Dalert);
        fprintf(whofile,"RECORD CALL: %s\n",phonelst.DRecordCall);
        fprintf(whofile,"SYSTEM TYPE: %s\n",phonelst.Dtype);
        fprintf(whofile,"\n");
        fclose(whofile);
	}
} 


#define BRIGHT_RED   RGB(255, 0, 0)
#define BRIGHT_GREEN RGB(0, 255, 0)
#define PURPLE       RGB(255, 0, 255)
#define BRIGHT_BLUE  RGB(0, 0, 255)
#define BLUE         RGB(0, 0, 100)
#define BLACK        RGB(0,0,0)


HBRUSH CCELLULARDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
    int nCtrl = pWnd->GetDlgCtrlID();
	switch (nCtrl)
	{
		case IDC_EDIT_STATUS:
			 pDC->SetTextColor(BRIGHT_BLUE); 
			 break;

	    case IDC_EDIT_CALLS:
 			 pDC->SetTextColor(BRIGHT_RED); 
			 break;

		case IDC_TEXT_TOTALCALL:
	    case IDC_TEXT_CALLSLEVEL6:
	    case IDC_TEXT_CALLSLEVEL5:
	    case IDC_TEXT_CALLSLEVEL4:
	    case IDC_TEXT_CALLSLEVEL3:
	    case IDC_TEXT_CALLSLEVEL2:
	    case IDC_TEXT_CALLSLEVEL1:
	    case IDC_TEXT_CALLSLEVEL0:
 		 	 pDC->SetTextColor(BLUE); 
			 break;
			 
	    case IDC_TEXT_CALLSLEVEL7:
	    case IDC_TEXT_CALLSLEVEL8:
	    case IDC_TEXT_CALLSLEVEL9:
	    case IDC_TEXT_CALLSRECORD:
        case IDC_TEXT_CALLSMODEM:
	    case IDC_TEXT_CALLSFAX:
 		 	 pDC->SetTextColor(BRIGHT_RED); 
			 break;

		default:
			break;
	}   

	return hbr;
}

void CCELLULARDlg::OnCheckAquire() 
{
	if(bAquire==false)
	{
        bAquire = true;
		return;
	}
    else
	{
        bAquire = false;
	    return;
	}
}

void CCELLULARDlg::OnCheckMonitor() 
{
	if(bMonitor==false)
	{
        bMonitor = true;
		return;
	}
    else
	{
        bMonitor = false;
	    return;
	}
}


void CCELLULARDlg::GoForwardVoice(void) 
{
     Sleep(250);

	 SetDDItoForward();

	 if(stanum ==0x54) stanum = 0x99;
     if(stanum ==0x53) stanum = 0x98;
     if(stanum ==0x52) stanum = 0x97;
     if(stanum ==0x51) stanum = 0x96;
     if(stanum ==0x50) stanum = 0x95;
     if(stanum ==0x49) stanum = 0x94;
     if(stanum ==0x48) stanum = 0x93;
     if(stanum ==0x47) stanum = 0x92;
     if(stanum ==0x46) stanum = 0x91;
     if(stanum ==0x45) stanum = 0x90;
     if(stanum ==0x44) stanum = 0x89;
     if(stanum ==0x43) stanum = 0x88;
     if(stanum ==0x42) stanum = 0x87;
     if(stanum ==0x41) stanum = 0x86;
     if(stanum ==0x40) stanum = 0x85;
     if(stanum ==0x39) stanum = 0x84;
     if(stanum ==0x38) stanum = 0x83;
     if(stanum ==0x37) stanum = 0x82;
     if(stanum ==0x36) stanum = 0x81;
     if(stanum ==0x35) stanum = 0x80;
     if(stanum ==0x34) stanum = 0x79;
     if(stanum ==0x33) stanum = 0x78;
     if(stanum ==0x32) stanum = 0x77;
     if(stanum ==0x31) stanum = 0x76;
     if(stanum ==0x30) stanum = 0x75;
     if(stanum ==0x29) stanum = 0x74;
     if(stanum ==0x28) stanum = 0x73;
     if(stanum ==0x27) stanum = 0x72;
     if(stanum ==0x26) stanum = 0x71;
     if(stanum ==0x25) stanum = 0x70;
     
     strcpy(chChannelType,"Forward Voice");
	 osfreq(endnum,stanum);

	 squelch=OPEN;
	
	 while(squelch==OPEN)
	 {
		 if(bCancelCall==true)
		 {
 	          bCancelCall=false;
			  WriteToStatusBox(" GOT SKIPPED VOICE COMMAND FROM SECONDARY FORWARD VOICE");
 			  break;
		 }
	 }	
}

void CCELLULARDlg::GoReverseVoice(void) 
{
     Sleep(250);

	 SetDDItoReverse();

	 if(stanum ==0x99) stanum = 0x54;
	 if(stanum ==0x98) stanum = 0x53;
	 if(stanum ==0x97) stanum = 0x52;
	 if(stanum ==0x96) stanum = 0x51;
	 if(stanum ==0x95) stanum = 0x50;
	 if(stanum ==0x94) stanum = 0x49;
	 if(stanum ==0x93) stanum = 0x48;
	 if(stanum ==0x92) stanum = 0x47;
	 if(stanum ==0x91) stanum = 0x46;
	 if(stanum ==0x90) stanum = 0x45;
	 if(stanum ==0x89) stanum = 0x44;
	 if(stanum ==0x88) stanum = 0x43;
	 if(stanum ==0x87) stanum = 0x42;
	 if(stanum ==0x86) stanum = 0x41;
	 if(stanum ==0x85) stanum = 0x40;
	 if(stanum ==0x84) stanum = 0x39;
	 if(stanum ==0x83) stanum = 0x38;
	 if(stanum ==0x82) stanum = 0x37;
 	 if(stanum ==0x81) stanum = 0x36;
	 if(stanum ==0x80) stanum = 0x35;
	 if(stanum ==0x79) stanum = 0x34;
	 if(stanum ==0x78) stanum = 0x33;
	 if(stanum ==0x77) stanum = 0x32;
	 if(stanum ==0x76) stanum = 0x31;
	 if(stanum ==0x75) stanum = 0x30;
	 if(stanum ==0x74) stanum = 0x29;
	 if(stanum ==0x73) stanum = 0x28;
	 if(stanum ==0x72) stanum = 0x27;
	 if(stanum ==0x71) stanum = 0x26;
	 if(stanum ==0x70) stanum = 0x25;
	 
	 strcpy(chChannelType,"Reverse Voice");
		
	 osfreq(endnum,stanum);
	 
	 squelch=OPEN;
	
	 while(squelch==OPEN)
	 {
		 if(bHoldCall==false)
		 {
          	    ossquelch();
				check_call_time();
         }
			
		 if(bCancelCall==true)
		 {
 	          bCancelCall=false;
			  WriteToStatusBox(" GOT SKIPPED VOICE COMMAND FROM REVERSE VOICE");
 			  break;
		 }
     }

     GoForwardVoice();
	 squelch=OPEN;
}


void CCELLULARDlg::UpdateChannelStats(void) 
{
    char chTempString[25];
    char *pchTempString = chTempString;
	sprintf(pchTempString,"8%x.%x0 Mhz",stanum,endnum);

	SetDlgItemText(IDC_TEXT_FREQUENCY,pchTempString);
	SetDlgItemText(IDC_TEXT_CHANNELTYPE,chChannelType);
}


void CCELLULARDlg::show_scm(void)
{
    ::ZeroMemory(chSCMstring, sizeof(chSCMstring));

 if (phonelst.Dscm[1]=='0')
	 {sprintf(chSCMstring,"666 Chan, Continuous, 3.0 Wts");}
 if (phonelst.Dscm[1]=='1')
	 {sprintf(chSCMstring,"666 Chan, Continuous, 1.2 Wts");}
 if (phonelst.Dscm[1]=='2')
	 {sprintf(chSCMstring,"666 Chan, Continuous, 0.6 Wts");}
 if (phonelst.Dscm[1]=='3')
	 {sprintf(chSCMstring,"666 Chan, Continuous, Resv Wts");}
 if (phonelst.Dscm[1]=='4')
	 {sprintf(chSCMstring,"666 Channels, VOX, 3.0 Watts");}
 if (phonelst.Dscm[1]=='5')
	 {sprintf(chSCMstring,"666 Channels, VOX, 1.2 Watts");}
 if (phonelst.Dscm[1]=='6')
	 {sprintf(chSCMstring,"666 Channels, VOX, 0.6 Watts");}
 if (phonelst.Dscm[1]=='7')
	 {sprintf(chSCMstring,"666 Channels, VOX, Resv Watts");}
 if (phonelst.Dscm[1]=='8')
	 {sprintf(chSCMstring,"832 Chan, Continuous, 3.0 Wts");}
 if (phonelst.Dscm[1]=='9')
	 {sprintf(chSCMstring,"832 Chan, Continuous, 1.2 Wts");}
 if (phonelst.Dscm[1]=='A')
	 {sprintf(chSCMstring,"832 Chan, Continuous, 0.6 Wts");}
 if (phonelst.Dscm[1]=='B')
	 {sprintf(chSCMstring,"832 Chan, Continuous, Resv Wts");}
 if (phonelst.Dscm[1]=='C')
	 {sprintf(chSCMstring,"832 Channels, VOX, 3.0 Watts");}
 if (phonelst.Dscm[1]=='D')
	 {sprintf(chSCMstring,"832 Channels, VOX, 1.2 Watts");}
 if (phonelst.Dscm[1]=='E')
	 {sprintf(chSCMstring,"832 Channels, VOX, 0.6 Watts");}
 if (phonelst.Dscm[1]=='F')
	 {sprintf(chSCMstring,"832 Channels, VOX, Resv Watts");}
}




void CCELLULARDlg::ConvertGoFrequency(void)
{
	if(chGoFrequency[1] == '0')
	{
    	if(chGoFrequency[2] == '0') {stanum=0x00;}
		if(chGoFrequency[2] == '1') {stanum=0x01;}
		if(chGoFrequency[2] == '2') {stanum=0x02;}
		if(chGoFrequency[2] == '3') {stanum=0x03;}
		if(chGoFrequency[2] == '4') {stanum=0x04;}
		if(chGoFrequency[2] == '5') {stanum=0x05;}
		if(chGoFrequency[2] == '6') {stanum=0x06;}
		if(chGoFrequency[2] == '7') {stanum=0x07;}
		if(chGoFrequency[2] == '8') {stanum=0x08;}
		if(chGoFrequency[2] == '9') {stanum=0x09;}
	}

	if(chGoFrequency[1] == '1')
	{
    	if(chGoFrequency[2] == '0') {stanum=0x10;}
		if(chGoFrequency[2] == '1') {stanum=0x11;}
		if(chGoFrequency[2] == '2') {stanum=0x12;}
		if(chGoFrequency[2] == '3') {stanum=0x13;}
		if(chGoFrequency[2] == '4') {stanum=0x14;}
		if(chGoFrequency[2] == '5') {stanum=0x15;}
		if(chGoFrequency[2] == '6') {stanum=0x16;}
		if(chGoFrequency[2] == '7') {stanum=0x17;}
		if(chGoFrequency[2] == '8') {stanum=0x18;}
		if(chGoFrequency[2] == '9') {stanum=0x19;}
	}

	if(chGoFrequency[1] == '2')
	{
    	if(chGoFrequency[2] == '0') {stanum=0x20;}
		if(chGoFrequency[2] == '1') {stanum=0x21;}
		if(chGoFrequency[2] == '2') {stanum=0x22;}
		if(chGoFrequency[2] == '3') {stanum=0x23;}
		if(chGoFrequency[2] == '4') {stanum=0x24;}
		if(chGoFrequency[2] == '5') {stanum=0x25;}
		if(chGoFrequency[2] == '6') {stanum=0x26;}
		if(chGoFrequency[2] == '7') {stanum=0x27;}
		if(chGoFrequency[2] == '8') {stanum=0x28;}
		if(chGoFrequency[2] == '9') {stanum=0x29;}
	}

	if(chGoFrequency[1] == '3')
	{
    	if(chGoFrequency[2] == '0') {stanum=0x30;}
		if(chGoFrequency[2] == '1') {stanum=0x31;}
		if(chGoFrequency[2] == '2') {stanum=0x32;}
		if(chGoFrequency[2] == '3') {stanum=0x33;}
		if(chGoFrequency[2] == '4') {stanum=0x34;}
		if(chGoFrequency[2] == '5') {stanum=0x35;}
		if(chGoFrequency[2] == '6') {stanum=0x36;}
		if(chGoFrequency[2] == '7') {stanum=0x37;}
		if(chGoFrequency[2] == '8') {stanum=0x38;}
		if(chGoFrequency[2] == '9') {stanum=0x39;}
	}

	if(chGoFrequency[1] == '4')
	{
    	if(chGoFrequency[2] == '0') {stanum=0x40;}
		if(chGoFrequency[2] == '1') {stanum=0x41;}
		if(chGoFrequency[2] == '2') {stanum=0x42;}
		if(chGoFrequency[2] == '3') {stanum=0x43;}
		if(chGoFrequency[2] == '4') {stanum=0x44;}
		if(chGoFrequency[2] == '5') {stanum=0x45;}
		if(chGoFrequency[2] == '6') {stanum=0x46;}
		if(chGoFrequency[2] == '7') {stanum=0x47;}
		if(chGoFrequency[2] == '8') {stanum=0x48;}
		if(chGoFrequency[2] == '9') {stanum=0x49;}
	}

	if(chGoFrequency[1] == '5')
	{
    	if(chGoFrequency[2] == '0') {stanum=0x50;}
		if(chGoFrequency[2] == '1') {stanum=0x51;}
		if(chGoFrequency[2] == '2') {stanum=0x52;}
		if(chGoFrequency[2] == '3') {stanum=0x53;}
		if(chGoFrequency[2] == '4') {stanum=0x54;}
		if(chGoFrequency[2] == '5') {stanum=0x55;}
		if(chGoFrequency[2] == '6') {stanum=0x56;}
		if(chGoFrequency[2] == '7') {stanum=0x57;}
		if(chGoFrequency[2] == '8') {stanum=0x58;}
		if(chGoFrequency[2] == '9') {stanum=0x59;}
	}

	if(chGoFrequency[1] == '6')
	{
    	if(chGoFrequency[2] == '0') {stanum=0x60;}
		if(chGoFrequency[2] == '1') {stanum=0x61;}
		if(chGoFrequency[2] == '2') {stanum=0x62;}
		if(chGoFrequency[2] == '3') {stanum=0x63;}
		if(chGoFrequency[2] == '4') {stanum=0x64;}
		if(chGoFrequency[2] == '5') {stanum=0x65;}
		if(chGoFrequency[2] == '6') {stanum=0x66;}
		if(chGoFrequency[2] == '7') {stanum=0x67;}
		if(chGoFrequency[2] == '8') {stanum=0x68;}
		if(chGoFrequency[2] == '9') {stanum=0x69;}
	}

	
	if(chGoFrequency[1] == '7')
	{
    	if(chGoFrequency[2] == '0') {stanum=0x70;}
		if(chGoFrequency[2] == '1') {stanum=0x71;}
		if(chGoFrequency[2] == '2') {stanum=0x72;}
		if(chGoFrequency[2] == '3'){stanum=0x73;}
		if(chGoFrequency[2] == '4') {stanum=0x74;}
		if(chGoFrequency[2] == '5') {stanum=0x75;}
		if(chGoFrequency[2] == '6')  {stanum=0x76;}
		if(chGoFrequency[2] == '7'){stanum=0x77;}
		if(chGoFrequency[2] == '8'){stanum=0x78;}
		if(chGoFrequency[2] == '9') {stanum=0x79;}
	 }
	
	if(chGoFrequency[1] == '8')
	{
		if (chGoFrequency[2] == '0'){stanum=0x80;}
		if (chGoFrequency[2] == '1')  {stanum=0x81;}
		if (chGoFrequency[2] == '2')  {stanum=0x82;}
		if (chGoFrequency[2] == '3'){stanum=0x83;}
		if (chGoFrequency[2] == '4') {stanum=0x84;}
		if (chGoFrequency[2] == '5') {stanum=0x85;}
		if (chGoFrequency[2] == '6')  {stanum=0x86;}
		if (chGoFrequency[2] == '7'){stanum=0x87;}
		if (chGoFrequency[2] == '8'){stanum=0x88;}
		if (chGoFrequency[2] == '9') {stanum=0x89;}
	 }
	
	 if(chGoFrequency[1] == '9')
	 {
		 if (chGoFrequency[2] == '0') {stanum=0x90;}
	     if (chGoFrequency[2] == '1')  {stanum=0x91;}
		 if (chGoFrequency[2] == '2')  {stanum=0x92;}
		 if (chGoFrequency[2] == '3'){stanum=0x93;}
		 if (chGoFrequency[2] == '4') {stanum=0x94;}
		 if (chGoFrequency[2] == '5') {stanum=0x95;}
		 if (chGoFrequency[2] == '6')  {stanum=0x96;}
		 if (chGoFrequency[2] == '7'){stanum=0x97;}
		 if (chGoFrequency[2] == '8'){stanum=0x98;}
		 if (chGoFrequency[2] == '9') {stanum=0x99;}
	 }
	
	 if(chGoFrequency[4] == '0')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x00;}
				if (chGoFrequency[5] == '1')  {endnum=0x01;}
				if (chGoFrequency[5] == '2')  {endnum=0x02;}
				if (chGoFrequency[5] == '3'){endnum=0x03;}
				if (chGoFrequency[5] == '4') {endnum=0x04;}
				if (chGoFrequency[5] == '5') {endnum=0x05;}
				if (chGoFrequency[5] == '6')  {endnum=0x06;}
				if (chGoFrequency[5] == '7'){endnum=0x07;}
				if (chGoFrequency[5] == '8'){endnum=0x08;}
				if (chGoFrequency[5] == '9') {endnum=0x09;}
			  }
			if(chGoFrequency[4] == '1')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x10;}
				if (chGoFrequency[5] == '1')  {endnum=0x11;}
				if (chGoFrequency[5] == '2')  {endnum=0x12;}
				if (chGoFrequency[5] == '3'){endnum=0x13;}
				if (chGoFrequency[5] == '4') {endnum=0x14;}
				if (chGoFrequency[5] == '5') {endnum=0x15;}
				if (chGoFrequency[5] == '6')  {endnum=0x16;}
				if (chGoFrequency[5] == '7'){endnum=0x17;}
				if (chGoFrequency[5] == '8'){endnum=0x18;}
				if (chGoFrequency[5] == '9') {endnum=0x19;}
			  }
			if(chGoFrequency[4] == '2')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x20;}
				if (chGoFrequency[5] == '1')  {endnum=0x21;}
				if (chGoFrequency[5] == '2')  {endnum=0x22;}
				if (chGoFrequency[5] == '3'){endnum=0x23;}
				if (chGoFrequency[5] == '4') {endnum=0x24;}
				if (chGoFrequency[5] == '5') {endnum=0x25;}
				if (chGoFrequency[5] == '6')  {endnum=0x26;}
				if (chGoFrequency[5] == '7'){endnum=0x27;}
				if (chGoFrequency[5] == '8'){endnum=0x28;}
				if (chGoFrequency[5] == '9') {endnum=0x29;}
			  }
			if(chGoFrequency[4] == '3')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x30;}
				if (chGoFrequency[5] == '1')  {endnum=0x31;}
				if (chGoFrequency[5] == '2')  {endnum=0x32;}
				if (chGoFrequency[5] == '3'){endnum=0x33;}
				if (chGoFrequency[5] == '4') {endnum=0x34;}
				if (chGoFrequency[5] == '5') {endnum=0x35;}
				if (chGoFrequency[5] == '6')  {endnum=0x36;}
				if (chGoFrequency[5] == '7'){endnum=0x37;}
				if (chGoFrequency[5] == '8'){endnum=0x38;}
				if (chGoFrequency[5] == '9') {endnum=0x39;}
			  }
			if(chGoFrequency[4] == '4')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x40;}
				if (chGoFrequency[5] == '1')  {endnum=0x41;}
				if (chGoFrequency[5] == '2')  {endnum=0x42;}
				if (chGoFrequency[5] == '3'){endnum=0x43;}
				if (chGoFrequency[5] == '4') {endnum=0x44;}
				if (chGoFrequency[5] == '5') {endnum=0x45;}
				if (chGoFrequency[5] == '6')  {endnum=0x46;}
				if (chGoFrequency[5] == '7'){endnum=0x47;}
				if (chGoFrequency[5] == '8'){endnum=0x48;}
				if (chGoFrequency[5] == '9') {endnum=0x49;}
			  }
			if(chGoFrequency[4] == '5')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x50;}
				if (chGoFrequency[5] == '1')  {endnum=0x51;}
				if (chGoFrequency[5] == '2')  {endnum=0x52;}
				if (chGoFrequency[5] == '3'){endnum=0x53;}
				if (chGoFrequency[5] == '4') {endnum=0x54;}
				if (chGoFrequency[5] == '5') {endnum=0x55;}
				if (chGoFrequency[5] == '6')  {endnum=0x56;}
				if (chGoFrequency[5] == '7'){endnum=0x57;}
				if (chGoFrequency[5] == '8'){endnum=0x58;}
				if (chGoFrequency[5] == '9') {endnum=0x59;}
			  }
			if(chGoFrequency[4] == '6')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x60;}
				if (chGoFrequency[5] == '1')  {endnum=0x61;}
				if (chGoFrequency[5] == '2')  {endnum=0x62;}
				if (chGoFrequency[5] == '3'){endnum=0x63;}
				if (chGoFrequency[5] == '4') {endnum=0x64;}
				if (chGoFrequency[5] == '5') {endnum=0x65;}
				if (chGoFrequency[5] == '6')  {endnum=0x66;}
				if (chGoFrequency[5] == '7'){endnum=0x67;}
				if (chGoFrequency[5] == '8'){endnum=0x68;}
				if (chGoFrequency[5] == '9') {endnum=0x69;}
			  }
			if(chGoFrequency[4] == '7')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x70;}
				if (chGoFrequency[5] == '1')  {endnum=0x71;}
				if (chGoFrequency[5] == '2')  {endnum=0x72;}
				if (chGoFrequency[5] == '3'){endnum=0x73;}
				if (chGoFrequency[5] == '4') {endnum=0x74;}
				if (chGoFrequency[5] == '5') {endnum=0x75;}
				if (chGoFrequency[5] == '6')  {endnum=0x76;}
				if (chGoFrequency[5] == '7'){endnum=0x77;}
				if (chGoFrequency[5] == '8'){endnum=0x78;}
				if (chGoFrequency[5] == '9') {endnum=0x79;}
			  }
			if(chGoFrequency[4] == '8')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x80;}
				if (chGoFrequency[5] == '1')  {endnum=0x81;}
				if (chGoFrequency[5] == '2')  {endnum=0x82;}
				if (chGoFrequency[5] == '3'){endnum=0x83;}
				if (chGoFrequency[5] == '4') {endnum=0x84;}
				if (chGoFrequency[5] == '5') {endnum=0x85;}
				if (chGoFrequency[5] == '6')  {endnum=0x86;}
				if (chGoFrequency[5] == '7'){endnum=0x87;}
				if (chGoFrequency[5] == '8'){endnum=0x88;}
				if (chGoFrequency[5] == '9') {endnum=0x89;}
			  }
			if(chGoFrequency[4] == '9')
			  {
				if (chGoFrequency[5] == '0') {endnum=0x90;}
				if (chGoFrequency[5] == '1')  {endnum=0x91;}
				if (chGoFrequency[5] == '2')  {endnum=0x92;}
				if (chGoFrequency[5] == '3'){endnum=0x93;}
				if (chGoFrequency[5] == '4') {endnum=0x94;}
				if (chGoFrequency[5] == '5') {endnum=0x95;}
				if (chGoFrequency[5] == '6')  {endnum=0x96;}
				if (chGoFrequency[5] == '7'){endnum=0x97;}
				if (chGoFrequency[5] == '8'){endnum=0x98;}
				if (chGoFrequency[5] == '9') {endnum=0x99;}
			  }

}


void CCELLULARDlg::ReadIniFile(void)
{
	char chDirectoryName[MAX_PATH+100];
	char *pchDirectoryName = chDirectoryName;
	::GetCurrentDirectory(MAX_PATH,pchDirectoryName);

	strcat(pchDirectoryName,"\\CELLULAR.INI");
	
    TRACE("INI Filename = %s\n",pchDirectoryName);
	
	char chTextString[255];
    char *pchTextString = chTextString;

    DDICOM:
    GetPrivateProfileString("SETTINGS","DDI COMM PORT","Not Found",pchTextString,17,pchDirectoryName);
    if(strcmp("Not Found",pchTextString)==0)
	{
        WritePrivateProfileString("SETTINGS","DDI COMM PORT","2",pchDirectoryName);
        goto DDICOM;
	}
    else
	{
        iCommPortDDI = atoi(pchTextString);
	    iCommPortDDI = iCommPortDDI - 1;
		TRACE("iCommPortDDI = %d\n",iCommPortDDI);
	} 

    OS456COM:
    GetPrivateProfileString("SETTINGS","OS456 COMM PORT","Not Found",pchTextString,17,pchDirectoryName);
    if(strcmp("Not Found",pchTextString)==0)
	{
        WritePrivateProfileString("SETTINGS","OS456 COMM PORT","1",pchDirectoryName);
        goto OS456COM;
	}
    else
	{
        iCommPortOS456 = atoi(pchTextString);
	    iCommPortOS456 = iCommPortOS456 - 1;
		TRACE("iCommPortOS456 = %d\n",iCommPortOS456);
	} 

    FREQUENCY:
    GetPrivateProfileString("SETTINGS","FREQUENCY","Not Found",pchTextString,17,pchDirectoryName);
    if(strcmp("Not Found",pchTextString)==0)
	{
        WritePrivateProfileString("SETTINGS","FREQUENCY","879.540",pchDirectoryName);
        goto FREQUENCY;
	}
    else
	{
    	strcpy(chFrequency,pchTextString);
        TRACE1("chFrequency = %s\n",chFrequency);

		strcpy(chGoFrequency,chFrequency);
		
		ConvertGoFrequency();
	} 

/*       
    REVERSE:
    bReverseChannel = false;
	GetPrivateProfileString("SETTINGS","REVERSE CHANNEL","Not Found",pchTextString,17,pchDirectoryName);
    if(strcmp("Not Found",pchTextString)==0)
	{
        WritePrivateProfileString("SETTINGS","REVERSE CHANNEL","NO",pchDirectoryName);
        goto REVERSE;
	}
    else
	{
        if(strcmpi(pchTextString,"YES")==0)
		{
            bReverseChannel = true;
		} 
        if(strcmpi(pchTextString,"Y")==0)
		{
            bReverseChannel = true;
		}
        if(strcmpi(pchTextString,"ENABLED")==0)
		{
            bReverseChannel = true;
		}
        if(strcmpi(pchTextString,"ENABLE")==0)
		{
            bReverseChannel = true;
		}
    	if(strcmpi(pchTextString,"ON")==0)
		{
            bReverseChannel = true;
		}
	} 
*/    

    MAXCALLTIME:
    GetPrivateProfileString("SETTINGS","MAX CALL TIME","Not Found",pchTextString,17,pchDirectoryName);
    if(strcmp("Not Found",pchTextString)==0)
	{
        WritePrivateProfileString("SETTINGS","MAX CALL TIME","1200",pchDirectoryName);
        goto MAXCALLTIME;
	}
    else
	{
        iMaxCallTime = atoi(pchTextString);
		TRACE("iMaxCallTime = %d\n",iMaxCallTime);
	} 

}

void CCELLULARDlg::OnButtonControlType() 
{
    m_EditCallList.SetWindowText(" ");
	m_EditCallList.Clear();

	if(bReverseChannel == false)
    {
		m_ButtonSwitchControlType.SetWindowText("Switch To Reverse Control");
    	strcpy(chFrequency,chReverseChannel);
		TRACE("chReverseChannel = %s\n",chReverseChannel);
    }
	else
	{
		m_ButtonSwitchControlType.SetWindowText("Switch To Forward Control");
    	strcpy(chFrequency,chForwardChannel);
		TRACE("chForwardChannel = %s\n",chForwardChannel);
	}

    which_channel();

	if(bReverseChannel == true)
    {
        SetDDItoReverse();
	}
	else
	{
        SetDDItoForward();
	}
    
	osfreq(endnum,stanum);
}


void CCELLULARDlg::OnButtonSwitchvoice() 
{
    if(bVoiceForward == true)
	{
		m_ButtonSwitchVoice.SetWindowText("Forward Voice");
        bVoiceForward = false;
		bGoReverseVoice = true;
	}
	else
	{
		m_ButtonSwitchVoice.SetWindowText("Reverse Voice");
        bVoiceForward = true;
		bGoForwardVoice = true;
	}
}


void CCELLULARDlg::OnButtonControlscan() 
{
    InitalizeOS456();

   	InitalizeDDI();

    char chOriginalFrequency[50];
	strcpy(chOriginalFrequency,chFrequency);

	SetDDItoForward();

	WriteToCallBox("FORWARD CONTROL CHANNEL SCAN (BAND 'A')");
    WriteToCallBox("[ACTIVE]  [CHANNEL]  [FREQUENCY]  [SIGNAL STRENGTH]");


    ScanControlChannel("879.990");
    ScanControlChannel("879.960");
    ScanControlChannel("879.930");
    ScanControlChannel("879.900");
    ScanControlChannel("879.870");
    ScanControlChannel("879.840");
    ScanControlChannel("879.810");
    ScanControlChannel("879.780");
    ScanControlChannel("879.750");
    ScanControlChannel("879.720");
    ScanControlChannel("879.690");
    ScanControlChannel("879.660");
    ScanControlChannel("879.630");
    ScanControlChannel("879.600");
    ScanControlChannel("879.570");
    ScanControlChannel("879.540");
    ScanControlChannel("879.510");
    ScanControlChannel("879.480");
    ScanControlChannel("879.450");
    ScanControlChannel("879.420");
    ScanControlChannel("879.390");

   	WriteToCallBox(" ");
   	WriteToCallBox("FORWARD CONTROL CHANNEL SCAN (BAND 'B')");
    WriteToCallBox("[ACTIVE]  [CHANNEL]  [FREQUENCY]  [SIGNAL STRENGTH]");

    ScanControlChannel("835.020");
    ScanControlChannel("835.050");
    ScanControlChannel("835.080");
    ScanControlChannel("835.110");
    ScanControlChannel("835.140");
    ScanControlChannel("835.170");
    ScanControlChannel("835.200");
    ScanControlChannel("835.230");
    ScanControlChannel("835.260");
    ScanControlChannel("835.290");
    ScanControlChannel("835.320");
    ScanControlChannel("835.350");
    ScanControlChannel("835.380");
    ScanControlChannel("835.410");
    ScanControlChannel("835.440");
    ScanControlChannel("835.470");
    ScanControlChannel("835.500");
    ScanControlChannel("835.530");
    ScanControlChannel("835.560");
    ScanControlChannel("835.590");
    ScanControlChannel("835.620");
    
    WriteToCallBox(" ");
    WriteToCallBox(" Channels mark with an X are active control channels.");
    WriteToCallBox(" The lower the signal strength number is the better the signal");
    WriteToCallBox(" 0 is a perfect signal. - 125 is the worst signal.");

	
	osspeakeroff();
   	strcpy(chFrequency,chOriginalFrequency);
 	which_channel();
    osfreq(endnum,stanum);
    
}

void CCELLULARDlg::ScanControlChannel(char *chFreq) 
{
    char chActive[5];
    char chTempString[250];
    char *pchTempString = chTempString;


	strcpy(chFrequency,chFreq);
    which_channel();
	osfreq(endnum,stanum);
    osspeakeron();
	Sleep(250);
	ossignalstr();
	if(strcmp(chSignalStrength,"- 125 dBm")==0)
	{
        strcpy(chActive,"  ");
	}
	else
	{
        strcpy(chActive,"X");
	}
	sprintf(pchTempString,"     [%s]          [%s]             [%s]            [%s]",chActive,chChannel,chFrequency,chSignalStrength);
    WriteToCallBox(pchTempString);
}


	
