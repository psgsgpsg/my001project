#if !defined(AFX_VERTIFYDLG_H__9C39E40F_F068_4FCD_BB31_02856B5C4E94__INCLUDED_)
#define AFX_VERTIFYDLG_H__9C39E40F_F068_4FCD_BB31_02856B5C4E94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VertifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVertifyDlg dialog

class CVertifyDlg : public CDialog
{
// Construction
public:
	CVertifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVertifyDlg)
	enum { IDD = IDD_VERTIFY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVertifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVertifyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	SetInformation(const CString info);
	
	
private:
	CString m_verityMsg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERTIFYDLG_H__9C39E40F_F068_4FCD_BB31_02856B5C4E94__INCLUDED_)
