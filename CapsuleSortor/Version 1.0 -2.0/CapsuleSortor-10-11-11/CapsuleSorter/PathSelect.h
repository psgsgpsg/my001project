#if !defined(AFX_PATHSELECT_H__A2316B89_F6F1_4202_97F0_55CDE2E52A2E__INCLUDED_)
#define AFX_PATHSELECT_H__A2316B89_F6F1_4202_97F0_55CDE2E52A2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPathSelect dialog

class CPathSelect : public CDialog
{
// Construction
public:
	CPathSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPathSelect)
	enum { IDD = IDD_DLG_PATHSELECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPathSelect)
	afx_msg void OnBtnDef();
	afx_msg void OnBtnSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHSELECT_H__A2316B89_F6F1_4202_97F0_55CDE2E52A2E__INCLUDED_)
