// CamConfigSetting.cpp : implementation file
//

#include "stdafx.h"
#include "CapsuleSorter.h"
#include "CapsuleSorterDlg.h"
#include "CamConfigSetting.h"
#include "KeyBoardDlg.h"
#include "configinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamConfigSetting dialog


CCamConfigSetting::CCamConfigSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CCamConfigSetting::IDD, pParent),
	m_firstPlayer(TBmpInfo::e8Bit),
	m_secondPlayer(TBmpInfo::e8Bit),
	m_FirstCard(0),
	m_SecondCard(0)

{
	//{{AFX_DATA_INIT(CCamConfigSetting)
	m_genieToGenie = 0.0f;
	m_genieToProci = 0.0f;
	m_prociToGenie = 0.0f;
	m_prociToProci = 0.0f;
	m_colorROIStartX = 0;
	m_colorROIStartY = 0;
	m_colorROIWidth = 0;
	m_colorROIHeight = 0;
	m_colorExpTime = 0;
	m_colorGain = 0;
	m_whiteRedVal = 0;
	m_whiteBlueVal = 0;
	m_monoROIWidth = 0;
	m_monoROIHeight = 0;
	m_monoROIStartY = 0;
	m_monoROIStartX = 0;
	m_monoGain = 0;
	m_monoExpTime = 0;
	m_maxValue = 0.0;
	//}}AFX_DATA_INIT
}


void CCamConfigSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamConfigSetting)
	DDX_Control(pDX, IDC_CamSelect, m_camSelect);
	DDX_Text(pDX, IDC_GenieToGenie, m_genieToGenie);
	DDX_Text(pDX, IDC_GenieToProci, m_genieToProci);
	DDX_Text(pDX, IDC_ProciToGenie, m_prociToGenie);
	DDX_Text(pDX, IDC_ProciToProci, m_prociToProci);
	DDX_Text(pDX, IDC_ROIStartX, m_colorROIStartX);
	DDX_Text(pDX, IDC_ROIStartY, m_colorROIStartY);
	DDX_Text(pDX, IDC_ROIWidth, m_colorROIWidth);
	DDX_Text(pDX, IDC_ROIHeight, m_colorROIHeight);
	DDX_Text(pDX, IDC_ExpTime, m_colorExpTime);
	DDX_Text(pDX, IDC_Gain, m_colorGain);
	DDX_Text(pDX, IDC_WhiteRedVal, m_whiteRedVal);
	DDX_Text(pDX, IDC_WhiteBlueVal, m_whiteBlueVal);
	DDX_Text(pDX, IDC_MONO_ROI_WIDTH, m_monoROIWidth);
	DDX_Text(pDX, IDC_MONO_ROI_HEIGHT, m_monoROIHeight);
	DDX_Text(pDX, IDC_MONO_ROI_STARTY, m_monoROIStartY);
	DDX_Text(pDX, IDC_MONO_ROI_STARTX, m_monoROIStartX);
	DDX_Text(pDX, IDC_MONO_GAIN, m_monoGain);
	DDX_Text(pDX, IDC_MONO_EXPTIME, m_monoExpTime);
	DDX_Text(pDX, IDC_RECORD, m_maxValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamConfigSetting, CDialog)
	//{{AFX_MSG_MAP(CCamConfigSetting)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CAMCAPTURE, OnCamcapture)
	ON_BN_CLICKED(IDC_CAMFREEZE, OnCamfreeze)
	ON_BN_CLICKED(IDC_SET_COLORCAM, OnSetColorcam)
	ON_BN_CLICKED(IDC_SET_MONOCAM, OnSetMonocam)
	ON_BN_CLICKED(IDC_GET_COLORCAM, OnGetColorcam)
	ON_BN_CLICKED(IDC_GET_MONOCAM, OnGetMonocam)
	ON_BN_CLICKED(IDC_CamSoftKeyBD, OnCamSoftKeyBD)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamConfigSetting message handlers

void CCamConfigSetting::ProciProcess(void *param)
{
	CCamConfigSetting* pThis = (CCamConfigSetting*)(param);
	{
		//pThis->UpdateData();
		//Calculate time difference
			
		//Image processing
		while (1)
		{
			pThis->m_genieToProci = pThis->m_imgTimeDiff.msec();
			pThis->m_prociToProci = (reinterpret_cast<FirstCam *>(pThis->m_FirstCard))->FrameInterval();
			
			pThis->m_FirstCard->Wait();
			void *pMemBase = pThis->m_FirstCard->GetPixelMemBase();
			if(pMemBase)
			{
				if(pThis->m_FirstCard->BytesPerPixel() == 1)
				{
					PelGray8 *pGrayNew	= (PelGray8*)pThis->m_firstImg.Base();
					memcpy(pGrayNew, pMemBase, pThis->m_firstImg.Size());				
				}
				else if(pThis->m_FirstCard->BytesPerPixel() == 3)
				{
					PelRGB24 *pRGBOld		= (PelRGB24*)(pMemBase);
					PelRGB24 *pRGBNew		= (PelRGB24*)pThis->m_firstImg.Base();
					size_t	  pixelCount	= pThis->m_FirstCard->Width() * pThis->m_FirstCard->Height();
					
					for(size_t index = 0; index < pixelCount; ++index)
					{
						pRGBNew->V[0]	= pRGBOld->B;
						pRGBNew->V[1]	= pRGBOld->G;
						pRGBNew->V[2]	= pRGBOld->R;
						
						++pRGBOld;
						++pRGBNew;
					}
				}
				
				pThis->m_firstPlayer.SetImage(	pThis->m_firstImg.Base(), 
					pThis->m_FirstCard->Width(),
					pThis->m_FirstCard->Height());
				
				pThis->InvalidateRect		 (pThis->m_colorRect, FALSE);
				
			}
			
			//Display time difference	
			CString data;
			data.Format("%.3f", pThis->m_prociToProci);
			pThis->SetDlgItemText(IDC_ProciToProci, data);
			data.Format("%.3f", pThis->m_genieToProci);
			pThis->SetDlgItemText(IDC_GenieToProci, data);
			{
				if (pThis->m_genieToProci > pThis->m_maxValue)
				{
					pThis->m_maxValue = pThis->m_genieToProci;
					CString txt;
					txt.Format("%.3f", pThis->m_maxValue);
					pThis->SetDlgItemText(IDC_RECORD, txt);				
				}
			}
		}
	}
		
}

void CCamConfigSetting::GenieProcess(void *param)
{
	CCamConfigSetting* pThis = (CCamConfigSetting*)(param);

	//Calculate time difference
	
	while (1)
	{
		pThis->m_genieToGenie = (reinterpret_cast<SecondCam *>(pThis->m_SecondCard))->FrameInterval();
		pThis->m_imgTimeDiff.Reset();
		pThis->m_prociToGenie = pThis->m_genieToGenie - pThis->m_genieToProci;
		pThis->m_SecondCard->Wait();
		
		//Image process
		void *pMemBase = pThis->m_SecondCard->GetPixelMemBase();
		if(pMemBase)
		{
			if(pThis->m_SecondCard->BytesPerPixel() == 1)
			{
				PelGray8 *pGrayNew	= (PelGray8*)pThis->m_secondImg.Base();
				memcpy(pGrayNew, pMemBase, pThis->m_secondImg.Size());
			}
			else if(pThis->m_SecondCard->BytesPerPixel() == 3)
			{
				PelRGB24 *pRGBOld		= (PelRGB24*)(pMemBase);
				PelRGB24 *pRGBNew		= (PelRGB24*)pThis->m_secondImg.Base();
				size_t	  pixelCount	= pThis->m_SecondCard->Width() * pThis->m_SecondCard->Height();
				
				for(size_t index = 0; index < pixelCount; ++index)
				{
					pRGBNew->V[0]	= pRGBOld->B;
					pRGBNew->V[1]	= pRGBOld->G;
					pRGBNew->V[2]	= pRGBOld->R;
					
					++pRGBOld;
					++pRGBNew;
				}
			}
			
			pThis->m_secondPlayer.SetImage(	pThis->m_secondImg.Base(), 
				pThis->m_SecondCard->Width(),
				pThis->m_SecondCard->Height());
			
			pThis->InvalidateRect		 (pThis->m_monoRect, FALSE);
			
		}
		//Display time difference
		CString data;
		data.Format("%.3f", pThis->m_genieToGenie);
		pThis->SetDlgItemText(IDC_GenieToGenie, data);
		data.Format("%.3f", pThis->m_prociToGenie);
		pThis->SetDlgItemText(IDC_ProciToGenie, data);
	}
	
}

BOOL CCamConfigSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Title
	CString title;
	title.LoadString(IDS_CAMERACFG_TITLE);
	SetWindowText(title);
	
	//Position
	RECT	rect;
	GetWindowRect(&rect);
	int	scWidth	= GetSystemMetrics(SM_CXSCREEN);
	int	scHeight= GetSystemMetrics(SM_CYSCREEN);
	int width	= rect.right  - rect.left;
	int height	= rect.bottom - rect.top;
	int left	= (scWidth - width)/2;
	int top		= 0;
	MoveWindow(left, top, width, height);
	ShowWindow(SW_SHOW);

	//Init
	CapsuleSorterDlg* mainDlg = (CapsuleSorterDlg*)AfxGetMainWnd();
	mainDlg->GetCamera(m_FirstCard, m_SecondCard);

	m_FirstCard->StopLive();
	m_SecondCard->StopLive();

	m_firstImg = TAlloc<FirstCamPixel>(m_FirstCard->Width() * m_SecondCard->Height());
	m_secondImg  = TAlloc<SecondCamPixel>(m_SecondCard->Width()  * m_SecondCard->Height());

	m_firstThread.SetThreadFunc(CCamConfigSetting::ProciProcess, this);
	m_secondThread.SetThreadFunc(CCamConfigSetting::GenieProcess, this);
	m_firstThread.Start(TThread::HIGHEST);
	m_secondThread.Start(TThread::HIGHEST);

//	m_FirstCard->RegisterProcFunc(CCamConfigSetting::ProciProcess, this);
//	m_SecondCard->RegisterProcFunc	(CCamConfigSetting::GenieProcess, this);

	GetDlgItem(IDC_MONO_RECT)->GetWindowRect(&m_monoRect);
	ScreenToClient(&m_monoRect);
	
	GetDlgItem(IDC_COLOR_RECT)->GetWindowRect(&m_colorRect);
	ScreenToClient(&m_colorRect);


	((CComboBox*)GetDlgItem(IDC_CamSelect))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCamConfigSetting::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	Display(dc.m_hDC);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CCamConfigSetting::OnCamcapture() 
{
	// TODO: Add your control notification handler code here
	bool colorOk	= false;
	bool monoOk		= false;
	
	ConfigInfo& conInf= ConfigInfo::TheConfigInfo();
	switch(m_camSelect.GetCurSel())
	{
	case 0:
		//m_colorImgCard->Open();
		m_FirstCard->Open(conInf.FirstCamInfo().camTag.c_str());
		m_SecondCard->Open(conInf.SecondCamInfo().camTag.c_str());
		colorOk	= m_FirstCard->StartLive();
		monoOk	= m_SecondCard->StartLive();
		if(!colorOk || !monoOk)
		{
			MessageBox("相机不能正常工作");
		}
		break;
		
	case 1:
		m_FirstCard->Open(conInf.FirstCamInfo().camTag.c_str());
		colorOk  = m_FirstCard->StartLive();
		if(!colorOk)
		{
			MessageBox("相机不能正常工作");
		}
		break;
		
	case 2:
		m_SecondCard->Open(conInf.SecondCamInfo().camTag.c_str());
		monoOk  = m_SecondCard->StartLive();
		if(!monoOk)
		{
			MessageBox("相机不能正常工作");
		}
		break;
		
	default:
		;
	}
	
}
void CCamConfigSetting::Display(HDC hdc)
{
	{
		CRect rect	= m_monoRect;
		size_t width	= m_SecondCard->Width();
		size_t height	= m_SecondCard->Height();
		m_secondPlayer.StrechToDevice(hdc, rect.left, rect.bottom - height/4, width/4, height/4,
			0, 0, width, height);
	}
	
	{
		CRect rect	= m_colorRect;
		size_t width	= m_FirstCard->Width();
		size_t height	= m_FirstCard->Height();
		m_firstPlayer.StrechToDevice(hdc,rect.left, rect.bottom - height/4, width/4, height/4,
			0, 0, width, height);
	}
}

BOOL CCamConfigSetting::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnCamfreeze();
		
	return CDialog::DestroyWindow();
}

void CCamConfigSetting::OnCamfreeze() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	switch(m_camSelect.GetCurSel())
	{
	case 0:
		m_SecondCard->StopLive();	
		m_FirstCard->StopLive();
		break;
	case 1:
		m_FirstCard->StopLive();
		break;
	case 2:
		m_SecondCard->StopLive();
		break;
	default:
		;
	}	
}

void CCamConfigSetting::OnSetColorcam() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!m_FirstCard->SetROI(m_colorROIStartX , m_colorROIStartY , m_colorROIWidth, m_colorROIHeight))
	{
		AfxMessageBox("设置感兴趣区域失败！");
	}
		CapsuleSorterDlg* mainDlg = (CapsuleSorterDlg*)AfxGetMainWnd();
	//mainDlg->InitImageCard();
	m_FirstCard->SetExpTime		(m_colorExpTime);
	m_FirstCard->SetGain		(m_colorGain);
	m_firstImg = TAlloc<FirstCamPixel>(m_FirstCard->Width() * m_FirstCard->Height());
	m_secondImg = TAlloc<SecondCamPixel>(m_SecondCard->Width()  * m_SecondCard->Height());
}

void CCamConfigSetting::OnSetMonocam() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!m_SecondCard->SetROI		(m_monoROIStartX , m_monoROIStartY , m_monoROIWidth, m_monoROIHeight))
	{
		AfxMessageBox("设置感兴趣区域失败！");
	}
	CapsuleSorterDlg* mainDlg = (CapsuleSorterDlg*)AfxGetMainWnd();
//	mainDlg->InitImageCard();
	m_SecondCard->SetExpTime	(m_monoExpTime);
	m_SecondCard->SetGain		(m_monoGain);	
}

void CCamConfigSetting::OnGetColorcam() 
{
	// TODO: Add your control notification handler code here
	size_t colorROIStartX, colorROIStartY,colorROIWidth,colorROIHeight;
	m_FirstCard->GetROI(colorROIStartX, colorROIStartY, colorROIWidth, colorROIHeight);
	size_t	colorGain, colorExpTime, whiteRedVal, whiteBlueVal, whiteGreenVal;
	
	m_FirstCard->GetGain(colorGain);
	m_FirstCard->GetExpTime(colorExpTime);
	m_FirstCard->GetWhiteBal(whiteRedVal, whiteGreenVal ,whiteBlueVal );
	
	m_colorROIStartX	= colorROIStartX;
	m_colorROIStartY	= colorROIStartY;
	m_colorROIWidth		= colorROIWidth;
	m_colorROIHeight	= colorROIHeight;
	m_colorGain			= colorGain;
	m_colorExpTime		= colorExpTime;
	m_whiteRedVal		= whiteRedVal;
	m_whiteBlueVal		= whiteBlueVal;
	UpdateData(FALSE);
}

void CCamConfigSetting::OnGetMonocam() 
{
	// TODO: Add your control notification handler code here
	size_t	ROIStartX, ROIStartY, ROIWidth, ROIHeight;
	m_SecondCard->GetROI(ROIStartX, ROIStartY , ROIWidth, ROIHeight);
	size_t	monoGain, monoExpTime;
	m_SecondCard->GetGain(monoGain);
	m_SecondCard->GetExpTime(monoExpTime);
	m_monoROIStartX = ROIStartX;
	m_monoROIStartY = ROIStartY;
	m_monoROIWidth	= ROIWidth;
	m_monoROIHeight = ROIHeight;
	m_monoGain		= monoGain;
	m_monoExpTime	= monoExpTime;
	UpdateData(FALSE);
}

void CCamConfigSetting::OnCamSoftKeyBD() 
{
	CKeyBoardDlg::DisplayWindow();
}


void CCamConfigSetting::OnButton2() 
{
	// TODO: Add your control notification handler code here
	m_maxValue = 0.0f;	
}
