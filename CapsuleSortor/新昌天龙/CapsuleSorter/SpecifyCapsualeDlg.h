#if !defined(AFX_SPECIFYCAPSUALEDLG_H__2B7F82EF_AADE_4415_A80E_D3FDA10448FD__INCLUDED_)
#define AFX_SPECIFYCAPSUALEDLG_H__2B7F82EF_AADE_4415_A80E_D3FDA10448FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecifyCapsualeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpecifyCapsualeDlg dialog

class CSpecifyCapsualeDlg : public CDialog
{
// Construction
public:
	CSpecifyCapsualeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpecifyCapsualeDlg)
	enum { IDD = IDD_SPECIFY_CAPSUAL_CFG };
	CString m_specifyNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecifyCapsualeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString	GetSpecifyNum();
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CSpecifyCapsualeDlg)
	afx_msg void OnChangeSpecifyCapsual();
	afx_msg void OnCfgSoftKeyBD();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECIFYCAPSUALEDLG_H__2B7F82EF_AADE_4415_A80E_D3FDA10448FD__INCLUDED_)
