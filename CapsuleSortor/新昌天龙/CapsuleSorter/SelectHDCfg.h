#if !defined(AFX_SELECTHDCFG_H__A1347BA0_A4EE_4EF4_BEBA_F8842E45D8E7__INCLUDED_)
#define AFX_SELECTHDCFG_H__A1347BA0_A4EE_4EF4_BEBA_F8842E45D8E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectHDCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHardWareConfig dialog

class CHardWareConfig : public CDialog
{
// Construction
public:
	CHardWareConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHardWareConfig)
	enum { IDD = IDD_HARDWARE_CONFIG };
	BOOL	m_transparent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardWareConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHardWareConfig)
	afx_msg void OnCamCfg();
	afx_msg void OnControlboardCfg();
	afx_msg void OnValveEnable();
	virtual BOOL OnInitDialog();
	afx_msg void OnSaveAll();
	afx_msg void OnWrongSave();
	afx_msg void OnFrontLightEnable();
	afx_msg void OnTransparent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetSaveButtonTitle( );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTHDCFG_H__A1347BA0_A4EE_4EF4_BEBA_F8842E45D8E7__INCLUDED_)
