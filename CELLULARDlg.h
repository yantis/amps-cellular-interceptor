// CELLULARDlg.h : header file
//

#if !defined(AFX_CELLULARDLG_H__DC5483E9_34E0_11D1_9DFF_0000C0083FB4__INCLUDED_)
#define AFX_CELLULARDLG_H__DC5483E9_34E0_11D1_9DFF_0000C0083FB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CCELLULARDlg dialog

class CCELLULARDlg : public CDialog
{
// Construction
public:
	CCELLULARDlg(CWnd* pParent = NULL);	// standard constructor
    static DWORD ThreadProc(CCELLULARDlg *);

// Dialog Data
	//{{AFX_DATA(CCELLULARDlg)
	enum { IDD = IDD_CELLULAR_DIALOG };
	CProgressCtrl	m_ProgressSignal;
	CButton	m_ButtonSwitchControlType;
	CButton	m_ButtonSwitchVoice;
	CEdit	m_EditCallList;
	CStatic	m_Text_CallsLevel9;
	CStatic	m_Text_CallsLevel8;
	CStatic	m_Text_CallsLevel7;
	CStatic	m_Text_CallsLevel6;
	CStatic	m_Text_CallsLevel5;
	CStatic	m_Text_CallsLevel4;
	CStatic	m_Text_CallsLevel3;
	CStatic	m_Text_CallsLevel2;
	CStatic	m_Text_CallsLevel1;
	CStatic	m_Text_CallsLevel0;
	CStatic	m_Text_CallsTotal;
	CStatic	m_Text_CallsRecorded;
	CStatic	m_Text_CallsModem;
	CStatic	m_Text_CallsFax;
	CEdit	m_Edit_TotalCalls;
	CEdit	m_EditSubject;
	CEdit	m_EditSignalStrength;
	CEdit	m_EditSCM;
	CEdit	m_EditRecordCall;
	CEdit	m_EditPhoneType;
	CEdit	m_EditPhoneModel;
	CEdit	m_EditPhone;
	CEdit	m_EditOccupation;
	CEdit	m_EditName2;
	CEdit	m_EditName;
	CEdit	m_EditModemCalls;
	CEdit	m_EditLinkedFile;
	CEdit	m_EditLastCall;
	CEdit	m_EditLanguage;
	CEdit	m_EditInfo3;
	CEdit	m_EditInfo2;
	CEdit	m_EditInfo1;
	CEdit	m_EditGender;
	CEdit	m_EditFaxCalls;
	CEdit	m_EditESN;
	CEdit	m_EditDtmf;
	CEdit	m_EditDialedNumber;
	CEdit	m_EditChannelAB;
	CEdit	m_EditCallTime;
	CEdit	m_EditStart;
	CEdit	m_EditCallEnd;
	CEdit	m_EditArea;
	CEdit	m_EditAlertLevel;
	CEdit	m_Edit_Status;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCELLULARDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HANDLE hThread;
    bool   bCancelThread;
    bool   bThreadClosed;
	HICON m_hIcon;

protected:
	void CreateMyThread(void);
	void ProcessThread(void);
	void InitalizeDDI(void);
	void WriteToStatusBox(char *pchString);
    void MyError(char *s);
    void WriteDebugLog(char *chLogText);
    void GetDDIdata(void);
    void AnalyzeDDIdata(void);
    void whattime(void);
    void ZeroMemVariables(void);
    void amend_record(void);
	void insert_record(void);
    void show_database(void);
    void search(void);
    void which_channel(void);
   
	void InitalizeOS456(void);
    void os456(void);
    void osmode(void);
    void oslocal(void);
    void osremote(void);
    void osfreq(int endnum,int stanum);
    int osfreqcheck(void);
    void osreadfreq(void);
    void osspeakeroff(void);
    void osspeakeron(void);
	void ossignalstr(void);
    void ossquelch(void);
    int osspeakercheck(void);
    void switch_to_voice(void);
    void SetDDIvoiceMode(void);
    void check_call_time(void);
    void SetDDItoForward(void);
	void SetDDItoControlChannel(void);
    void CopyEditFieldsToDataBase(void);
    void WriteToCallBox(char *pchString);
    void UpdateCallList(void);
    void WriteWhoFile(int iFilenum,char *pchTotalTime);
    void UpdateCallList(int iWaveFileNumber,char *pchTotalTime);	
    void GoForwardVoice(void);
    void GoReverseVoice(void);
    void SetDDItoReverse(void);
    void UpdateChannelStats(void); 
    void show_scm(void);
    void ConvertGoFrequency(void);
    void ReadIniFile(void);
    void ScanControlChannel(char *chFreq);

	
	// Generated message map functions
	//{{AFX_MSG(CCELLULARDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnErrspaceEditStatus();
	afx_msg void OnButtonSkipvoicecall();
	afx_msg void OnButtonSaveData();
	afx_msg void OnButtonFindPhone();
	afx_msg void OnCheckHoldcall();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckAquire();
	afx_msg void OnCheckMonitor();
	afx_msg void OnButtonSwitchvoice();
	afx_msg void OnButtonControlType();
	afx_msg void OnButtonControlscan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLULARDLG_H__DC5483E9_34E0_11D1_9DFF_0000C0083FB4__INCLUDED_)
