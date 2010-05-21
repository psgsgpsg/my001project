// ImageBrowser.h : main header file for the ImageBrowser application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CImageBrowserApp:
// See ImageBrowser.cpp for the implementation of this class
//

class CImageBrowserApp : public CWinApp
{
public:
	CImageBrowserApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
};

extern CImageBrowserApp theApp;