// CapsuleSorterDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_CAPSULESORTERDLG_H__5A78E132_D9D1_4FA3_BCA3_CA1FF2DDB185__INCLUDED_)
#define AFX_CAPSULESORTERDLG_H__5A78E132_D9D1_4FA3_BCA3_CA1FF2DDB185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TImgBuffer.h"
#include "TImgPlayer.h"

#include "Valve.h"
#include "TBeacon.h"

#include "colorbutton.h"

#include <string>
#include "TSIMDChecker.h"
#include "TCritSect.h"

#include "softwareconfig.h"
#include "ctrlcardcomm.h"

#include "TIniFile.h"

/////////////////////////////////////////////////////////////////////////////
// CapsuleSorterDlg dialog

//typedef ProciliCard	GenieCard;

class CapsuleSorterDlg : public CDialog
{
// Construction
public:
	CapsuleSorterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CapsuleSorterDlg)
	enum { IDD = IDD_CAPSULESORTER_DIALOG };
	CStatic	m_yellowLight;
	CStatic	m_redLight;
	CStatic	m_greenLight;
	double	m_testResult;
	long	m_allCount;
	long	m_badCount;	
	BOOL	m_isSim;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CapsuleSorterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CapsuleSorterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLoadImage();
	afx_msg void OnGrabImage();
	afx_msg void OnDestroy();
	afx_msg void OnFreezeImg();
	afx_msg void OnClearCount();
	afx_msg void OnChangeTestSpec();
	afx_msg void OnSaveImage();
	afx_msg void OnSaveMultiImage();
	afx_msg void OnLoadMultiImage();
	afx_msg void OnIsSim();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSoftcfg();
	afx_msg void OnHardcfg();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnSoftKeyBD();
	afx_msg void OnLoadConfigFile();
	afx_msg void OnSaveConfigFile();
	afx_msg void OnStartComtest();
	afx_msg void OnEndComtest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void			GetCamera		(	FirstCam*		&m_proCam, 
										SecondCam*		&m_genCam);
	CapsuleProc*	GetProcessor	(	const TImgDim&	imgDim);
	CapsuleProc*	GetProcessor	(	size_t			colorChannelCount);
	void			InitImageCard	();

private:
	static	void	FirstCamFunc	(	void	*param);
	static	void	SecondCamFunc	(	void	*param);
	static	void	BeaconFunc		(	void	*param);

	void			IsBadHigh		();
	void			Display			(	HDC		hdc);
	void			DispCVBImg		(	IMG		imgIn,
										HDC		hdc,
										RECT	&rect);
	bool			UpdateConfig	(	const TString		&iniFile,
										TIniFile::EnumPath	ePath);
	
	std::string&	GetExeFileDir	();
	
	bool			InitComPort		();
	void			OnCancel();
private:
	enum{	THREADCOUNT = 2	};
	void			ConfigInterFace	();
	void			ChangeSolution	(const int width, const int height);

	SimularCard		m_simCam;				//模拟相机
	SimularCard		m_secondSimCam;			
	FirstCam		m_firstCam;				//prosilica相机对象
	SecondCam		m_secondCam;			//genie相机对象

	SecondProcess	m_secondProcess;		//二号相机处理
	FirstProcess	m_firstProcess;			//一号相机处理

	TThread			m_firstThread[THREADCOUNT];
	TThread			m_secondThread[THREADCOUNT];
	TCritSect		m_firstSect;
	TCritSect		m_secondSect;

	IMG				m_cvbHdlDisp;
	TThread			m_beaconThread;
	CCButton		m_colorbutton[15];

	int				m_solution_X;
	int				m_solution_Y;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPSULESORTERDLG_H__5A78E132_D9D1_4FA3_BCA3_CA1FF2DDB185__INCLUDED_)





















