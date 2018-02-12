// CELLULAR.h : main header file for the CELLULAR application
//

#if !defined(AFX_CELLULAR_H__DC5483E7_34E0_11D1_9DFF_0000C0083FB4__INCLUDED_)
#define AFX_CELLULAR_H__DC5483E7_34E0_11D1_9DFF_0000C0083FB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCELLULARApp:
// See CELLULAR.cpp for the implementation of this class
//

class CCELLULARApp : public CWinApp
{
public:
	CCELLULARApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCELLULARApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCELLULARApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLULAR_H__DC5483E7_34E0_11D1_9DFF_0000C0083FB4__INCLUDED_)
