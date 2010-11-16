#if !defined(AFX_CONTROLBOARDSETTING_H__FC5337AF_7AED_44E6_919B_FBF932AE30E8__INCLUDED_)
#define AFX_CONTROLBOARDSETTING_H__FC5337AF_7AED_44E6_919B_FBF932AE30E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlBoardSetting.h : header file
//
#include "CtrlCardComm.h"

/////////////////////////////////////////////////////////////////////////////
// CControlBoardSetting dialog

class CControlBoardSetting : public CDialog
{
// Construction
public:
	CControlBoardSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CControlBoardSetting)
	enum { IDD = IDD_CTRLBOARD_CONFIG };
	CComboBox	m_camInter_selection2;
	CComboBox	m_camInter_selection1;
	CComboBox	m_lamp_selection;
	CComboBox	m_valve_index;
	CComboBox	m_workMode;
	CComboBox	m_watchdog_mode;
	long	m_camDelay;
	long	m_camHold;
	long	m_lightDelay;
	long	m_lightHold;
	long	m_shieldTime;
	long	m_valveDelay;
	long	m_valveHold;
	long	m_camValveDistance1;
	long	m_camValveDistance2;
	long	m_axisRadius;
	long	m_codeAdjust;
	long	m_codePrecision;
	long	m_pulseInterval;
	long	m_capsuleInterval;
	CString	m_sensorIndex;
	CString	m_camIndex;
	CString	m_valveIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlBoardSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CheckValue(size_t curValue, size_t stdValue);

	// Generated message map functions
	//{{AFX_MSG(CControlBoardSetting)
	afx_msg void OnSetTimeorder();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetComplex();
	afx_msg void OnTestValve();
	afx_msg void OnTestLamp();
	afx_msg void OnSetSpace();
	afx_msg void OnSetOther();
	afx_msg void OnSetValue();
	afx_msg void OnReadCurrent();
	afx_msg void OnCtrlSoftKeyBD();
	afx_msg void OnSelchangeCamComb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLBOARDSETTING_H__FC5337AF_7AED_44E6_919B_FBF932AE30E8__INCLUDED_)
