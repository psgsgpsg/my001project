// CapsuleSorter.h : main header file for the CAPSULESORTER application
//

#if !defined(AFX_CAPSULESORTER_H__296C9596_E2F0_48F5_B941_EC052CF3429E__INCLUDED_)
#define AFX_CAPSULESORTER_H__296C9596_E2F0_48F5_B941_EC052CF3429E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CapsuleSorterApp:
// See CapsuleSorter.cpp for the implementation of this class
//

class CapsuleSorterApp : public CWinApp
{

public:
	CapsuleSorterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CapsuleSorterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CapsuleSorterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPSULESORTER_H__296C9596_E2F0_48F5_B941_EC052CF3429E__INCLUDED_)
