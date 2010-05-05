// VideoAnalysis.h : main header file for the VIDEOANALYSIS application
//

#if !defined(AFX_VIDEOANALYSIS_H__06CB1295_CE21_47BB_A52E_5B19F85E110E__INCLUDED_)
#define AFX_VIDEOANALYSIS_H__06CB1295_CE21_47BB_A52E_5B19F85E110E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVideoAnalysisApp:
// See VideoAnalysis.cpp for the implementation of this class
//

class CVideoAnalysisApp : public CWinApp
{
public:
	CVideoAnalysisApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoAnalysisApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVideoAnalysisApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOANALYSIS_H__06CB1295_CE21_47BB_A52E_5B19F85E110E__INCLUDED_)
