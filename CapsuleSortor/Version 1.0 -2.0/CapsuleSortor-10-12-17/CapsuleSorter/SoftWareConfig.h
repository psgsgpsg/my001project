//{{AFX_INCLUDES()
#include "cvdisplay.h"
//}}AFX_INCLUDES
#if !defined(AFX_SOFTWARECONFIG_H__D41B5104_BEDB_4758_A459_89D5A81F6E82__INCLUDED_)
#define AFX_SOFTWARECONFIG_H__D41B5104_BEDB_4758_A459_89D5A81F6E82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoftWareConfig.h : header file
//
#include "TImgbuffer.h"
#include "icvcimg.h"
#include "ColorProcess.h"
#include "MonoProcess.h"
#include "CapsuleProc.h"
#include "TImgBuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CSoftWareConfig dialog

class CSoftWareConfig : public CDialog
{
// Construction
public:
	CSoftWareConfig(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSoftWareConfig)
	enum { IDD = IDD_SOFTWARE_CONFIG };
	CScrollBar	m_minboxScroll;
	CScrollBar	m_radiusScroll;
	CListCtrl	m_paramList;
	CCVdisplay	m_cvDisplay;
	UINT	m_colorBright;
	UINT	m_monoBright;
	BOOL	m_profileEnable;
	int		m_radius;
	long	m_minboxWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoftWareConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	enum { PARAMN=0, WIDTH, HEIGHT,	DIFF0, BODYLENGTH, CAPLENGTH, TOLERANCE };
	
	std::vector<CapsuleParam>	m_dimVect;
	std::vector<CapsuleParam>	m_dimVectPerFrame;
	std::vector<PelLab24>		m_hue1Vect;
	std::vector<PelLab24>		m_hue2Vect;
	std::vector<PelLab24>		m_hue1VectPerFrame;
	std::vector<PelLab24>		m_hue2VectPerFrame;

	CapsuleParam				m_curParam;

	TImgBuffer					m_imgBuff;
	TImgBuffer					m_imgPlayBuff;
	IMG							m_IMGForPlayer;

	CapsuleProc*				m_pProcessor;

	CapsuleParam				m_firstCapsuleParam;
	CapsuleParam				m_secondCapsuleParam;
	
	bool					DisplayImage		();	
	bool					Process				(	const TImgBuffer				&imgBuff);
	PelLab24				GetStdHue			(	const std::vector<PelLab24>		&hueVect);
	CapsuleParam			GetStdDim			(	const std::vector<CapsuleParam>	&dimVect);
	void					SetListContent		(	size_t							rowIndex, 
													const CapsuleParam				&param);
	CapsuleParam			GetListContent		(	size_t							rowIndex);
	bool					LoadMutiFrameImage	(	LPCSTR							filePath);

protected:

	// Generated message map functions
	//{{AFX_MSG(CSoftWareConfig)
	virtual BOOL OnInitDialog();
	afx_msg void OnLoadImage();
	afx_msg void OnNextFrame();
	afx_msg void OnDropResult();
	afx_msg void OnComplete();
	afx_msg void OnCapSoftKeyBD();
	afx_msg void OnChangeHue2();
	afx_msg void OnChangeHue1();
	afx_msg void OnChangeSizeChange2();
	afx_msg void OnChangeSizeChange1();
	afx_msg void OnProfile();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void	InitInterface(CRect	&rect);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOFTWARECONFIG_H__D41B5104_BEDB_4758_A459_89D5A81F6E82__INCLUDED_)
