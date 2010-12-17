#if !defined(AFX_CAMCONFIGSETTING_H__2C96A01D_7E83_471C_B48E_F10EB1694688__INCLUDED_)
#define AFX_CAMCONFIGSETTING_H__2C96A01D_7E83_471C_B48E_F10EB1694688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamConfigSetting.h : header file
//
#include "TImgPlayer.h"
#include "TTimediff.h"
#include "TypeDefine.h"
#include "TThread.h"

/////////////////////////////////////////////////////////////////////////////
// CCamConfigSetting dialog

class CCamConfigSetting : public CDialog
{
// Construction
public:
	CCamConfigSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCamConfigSetting)
	enum { IDD = IDD_CAMERA_CONFIG };
	CComboBox	m_camSelect;
	double	m_genieToGenie;
	double	m_genieToProci;
	double	m_prociToGenie;
	double	m_prociToProci;
	long	m_colorROIStartX;
	long	m_colorROIStartY;
	long	m_colorROIWidth;
	long	m_colorROIHeight;
	long	m_colorExpTime;
	long	m_colorGain;
	long	m_whiteRedVal;
	long	m_whiteBlueVal;
	long	m_monoROIWidth;
	long	m_monoROIHeight;
	long	m_monoROIStartY;
	long	m_monoROIStartX;
	long	m_monoGain;
	long	m_monoExpTime;
	double	m_maxValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamConfigSetting)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void				Display	 (HDC hdc);
	static		void	GenieProcess(void *param);
	static		void	ProciProcess(void *param);

	CRect				m_monoRect;
	CRect				m_colorRect;
	
	FirstCam*			m_FirstCard;	
	SecondCam*			m_SecondCard;

	TImgPlayer			m_secondPlayer;
	TImgPlayer			m_firstPlayer;
	
	TAlloc<FirstCamPixel>	        m_firstImg;
	TAlloc<SecondCamPixel>		m_secondImg;

	TTimeDiff			m_imgTimeDiff;
	TThread				m_firstThread;
	TThread				m_secondThread;

protected:

	// Generated message map functions
	//{{AFX_MSG(CCamConfigSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCamcapture();
	afx_msg void OnCamfreeze();
	afx_msg void OnSetColorcam();
	afx_msg void OnSetMonocam();
	afx_msg void OnGetColorcam();
	afx_msg void OnGetMonocam();
	afx_msg void OnCamSoftKeyBD();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMCONFIGSETTING_H__2C96A01D_7E83_471C_B48E_F10EB1694688__INCLUDED_)
