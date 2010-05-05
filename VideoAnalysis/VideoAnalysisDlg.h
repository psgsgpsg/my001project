// VideoAnalysisDlg.h : header file
//

#if !defined(AFX_VIDEOANALYSISDLG_H__4206018F_5DDE_4285_97CC_336833B691F7__INCLUDED_)
#define AFX_VIDEOANALYSISDLG_H__4206018F_5DDE_4285_97CC_336833B691F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVideoAnalysisDlg dialog
#include "timagetmpl.h"
#include "TImgArchive.h"

class CVideoAnalysisDlg : public CDialog
{
// Construction
public:
	CVideoAnalysisDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVideoAnalysisDlg)
	enum { IDD = IDD_VIDEOANALYSIS_DIALOG };
	CProgressCtrl	m_prgc_gain;
	CProgressCtrl	m_prgc_gray;
	CProgressCtrl	m_prgc_lght;
	CProgressCtrl	m_prgc_snr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoAnalysisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVideoAnalysisDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLoadimage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOANALYSISDLG_H__4206018F_5DDE_4285_97CC_336833B691F7__INCLUDED_)
