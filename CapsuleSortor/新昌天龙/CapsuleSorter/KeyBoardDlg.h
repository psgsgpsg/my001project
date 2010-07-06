#if !defined(AFX_KEYBOARDDLG_H__05100BDB_EDFB_4665_8DE7_639C6170B55C__INCLUDED_)
#define AFX_KEYBOARDDLG_H__05100BDB_EDFB_4665_8DE7_639C6170B55C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyBoardDlg.h : header file
//

#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardDlg dialog

class CKeyBoardDlg : public CDialog
{
// Construction
public:
static bool DisplayWindow(bool isSimulate = FALSE);

// Dialog Data
	//{{AFX_DATA(CKeyBoardDlg)
	enum { IDD = IDD_KEYBOARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyBoardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyBoardDlg)
	afx_msg void OnClose();
	afx_msg void OnBtnEsc();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool SendKeyValue(unsigned char keyVal);
	bool HandleBtnMsg(WPARAM wParam);
	CCButton	m_colorbutton[15];

private:
	CKeyBoardDlg(CWnd* pParent = NULL);   // standard constructor
	static CKeyBoardDlg* m_dlg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDDLG_H__05100BDB_EDFB_4665_8DE7_639C6170B55C__INCLUDED_)
