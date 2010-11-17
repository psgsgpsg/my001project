// CapsuleSorterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CapsuleSorter.h"
#include "CapsuleSorterDlg.h"
#include "icvcimg.h"
#include "TTimeDiff.h"
#include "TWinUtility.h"
#include "ColorButton.h"
#include "camconfigsetting.h"
#include "controlboardsetting.h"
#include "softwareconfig.h"
#include "selecthdcfg.h"
#include <string>
#include "TSIMDChecker.h"
#include "TMfdArchive.h"
#include "TImgArchive.h"
#include "KeyBoardDlg.h"
#include "VertifyDlg.h"
#include "ConfigInfo.h"
#include "SpecifyCapsualeDlg.h"
#include "pathselect.h"

#pragma comment(lib, "cvcimg.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CapsuleSorterDlg dialog

int SortorParam::transparent = 0;

CapsuleSorterDlg::CapsuleSorterDlg(CWnd* pParent /*=NULL*/)
:	CDialog(CapsuleSorterDlg::IDD, pParent),
	m_cvbHdlDisp(0)
{
	//{{AFX_DATA_INIT(CapsuleSorterDlg)
	m_testResult = 0.0;
	m_allCount = 0;
	m_badCount	= 0;
	m_isSim = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CapsuleSorterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CapsuleSorterDlg)
	DDX_Control(pDX, IDC_YELLOW, m_yellowLight);
	DDX_Control(pDX, IDC_RED, m_redLight);
	DDX_Control(pDX, IDC_GREEN, m_greenLight);
	DDX_Text(pDX, IDC_TestResult, m_testResult);
	DDX_Text(pDX, IDC_AllCount, m_allCount);
	DDX_Check(pDX, IDC_IsSim, m_isSim);
	DDX_Text(pDX, IDC_BadCount, m_badCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CapsuleSorterDlg, CDialog)
	//{{AFX_MSG_MAP(CapsuleSorterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LoadImage, OnLoadImage)
	ON_BN_CLICKED(IDC_GrabImage, OnGrabImage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FreezeImg, OnFreezeImg)
	ON_BN_CLICKED(IDC_ClearCount, OnClearCount)
	ON_EN_CHANGE(IDC_TestTarget, OnChangeTestSpec)
	ON_BN_CLICKED(IDC_SaveImage, OnSaveImage)
	ON_BN_CLICKED(IDC_SaveMultImg, OnSaveMultiImage)
	ON_BN_CLICKED(IDC_LoadMultImg, OnLoadMultiImage)
	ON_BN_CLICKED(IDC_IsSim, OnIsSim)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SOFTCFG, OnSoftcfg)
	ON_BN_CLICKED(IDC_HARDCFG, OnHardcfg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SOFTKEYBD, OnSoftKeyBD)
	ON_BN_CLICKED(IDC_LoadConfigFile, OnLoadConfigFile)
	ON_BN_CLICKED(IDC_SaveConfigFile, OnSaveConfigFile)
	ON_BN_CLICKED(IDC_START_COMTEST, OnStartComtest)
	ON_BN_CLICKED(IDC_END_COMTEST, OnEndComtest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CapsuleSorterDlg message handlers

BOOL CapsuleSorterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	size_t	scWidth		= GetSystemMetrics(SM_CXSCREEN);
	size_t	scHeight	= GetSystemMetrics(SM_CYSCREEN);

	RECT displayRect = {0, 0, scWidth, scHeight};

	MoveWindow(&displayRect, TRUE);
	
	UpdateConfig("Capsule.ini", TIniFile::eExeDir);

	m_solution_X = GetSystemMetrics(SM_CXSCREEN);
	m_solution_Y = GetSystemMetrics(SM_CYSCREEN);

	ConfigInfo&	cfgInfo = ConfigInfo::TheConfigInfo();
	if (m_solution_X != cfgInfo.Solution_X() || m_solution_Y != cfgInfo.Solution_Y())
	{
		ChangeSolution(cfgInfo.Solution_X(), cfgInfo.Solution_Y());
	}

	//comm
	if (!InitComPort())
	{
		if (!cfgInfo.WorkMode())
		{
			PostMessage(WM_QUIT);
	 		return FALSE;	
		}			
	}	
	
	
	//Image Card
	InitImageCard();

	//interface
	ConfigInterFace();
	OnSoftKeyBD(); 
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CapsuleSorterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CapsuleSorterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		Display(dc.m_hDC);	
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CapsuleSorterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/*******************************************************************************
*	�������ƣ�	OnGrabImage
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	����ȡ�µ�ͼ��
*
*
********************************************************************************/
void CapsuleSorterDlg::OnGrabImage() 
{
	if(m_isSim)
	{
		if(!m_simCam.StartLive())
		{
			MessageBox("Cannot Start simCam!");
		}		
	}
	else
	{
		ConfigInfo&	conInf = ConfigInfo::TheConfigInfo();
		if((!m_secondCam.IsLiving()))
		{
			m_secondCam.RegisterProcFunc(CapsuleSorterDlg::SecondCamFunc, this);
			if(m_secondCam.Open(conInf.SecondCamInfo().camTag.c_str()))
			{
				TImgDim dim = m_secondCam.ImageDimension();
				m_secondProcess.ReInit(dim);
			}
			if(!m_secondCam.StartLive())
			{
				MessageBox("Second Camera cannot Live!");
			}
		}	
		
		
		if(!m_firstCam.IsLiving())
		{
			m_firstCam.RegisterProcFunc	(CapsuleSorterDlg::FirstCamFunc, this);
			if(m_firstCam.Open(conInf.FirstCamInfo().camTag.c_str()))
			{
				TImgDim dim = m_firstCam.ImageDimension();
				m_firstProcess.ReInit(dim);
			}
			if(!m_firstCam.StartLive())
			{
				MessageBox("First Camera cannot Live!");
			}
		}
	}
	Sleep(0);
}
/*******************************************************************************
*	�������ƣ�	OnFreezeImg
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	������ͼ��ֹͣ�����̵߳Ĵ���
*
*
********************************************************************************/
void CapsuleSorterDlg::OnFreezeImg() 
{
	if(!m_simCam.StopLive())
	{
		MessageBox("Cannot Stop simCam!");
	}
	if(!m_secondSimCam.StopLive())
	{
		MessageBox("Cannot Stop SecondSimCam!");
	}	

	if((!m_firstCam.StopLive()))// || (!m_proCam.Close()))
	{
		MessageBox("Cannot Stop proCam!");
	}
	if((!m_secondCam.StopLive())) //|| (!m_genCam.Close())
	{
		MessageBox("Genie Camera cannot Freeze!");
	}
}

/*******************************************************************************
*	�������ƣ�	BeaconFunc
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	���������ƣ����ݳ��������õı�־���Ƶ���
*
*
********************************************************************************/

void CapsuleSorterDlg::BeaconFunc(void *param)
{
	static HICON LTGreen	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_LT_GREEN));
	static HICON LTYellow	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_LT_YELLOW));
	static HICON LTRed		= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_LT_RED));
	static HICON DKGreen	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_DK_GREEN));
	static HICON DKYellow	= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_DK_YELLOW));
	static HICON DKRed		= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_DK_RED));
	
	static bool Glight		= true;
	static bool Ylight		= false;
	static bool Rlight		= false;

	while(1)
	{
		CapsuleSorterDlg* pThis = (CapsuleSorterDlg*)(param);
		TBeacon& beacon		= TBeacon::TheBeacon();
		
		bool Gshine = beacon.GreenShining();
		bool Yshine = beacon.YellowShining();
		bool Rshine = beacon.RedShining();
		if(Glight != Gshine)
		{
			Glight = Gshine;
			if(Glight)	{	pThis->m_greenLight.SetIcon	(LTGreen);	}
			else		{	pThis->m_greenLight.SetIcon	(DKGreen);	}
		}
		if(Ylight != Yshine)
		{
			Ylight = Yshine;
			if(Ylight)	{	pThis->m_yellowLight.SetIcon(LTYellow);	}
			else		{	pThis->m_yellowLight.SetIcon(DKYellow);	}
		}
		if(Rlight != Rshine)
		{
			Rlight = Rshine;
			if(Rlight)	{	pThis->m_redLight.SetIcon(LTRed);		}
			else		{	pThis->m_redLight.SetIcon(DKRed);		}
		}
		
		beacon.Check();
		Sleep(1000);
	}
}

/*******************************************************************************
*	�������ƣ�	Display
*	
*	����	��HDC	
*			  
*			  
*	���	����
*
*	����	����ʾͼ�񣬸��½�����ʾ��ͼ��
*
********************************************************************************/

void CapsuleSorterDlg::Display(HDC hdc)
{
	{
		CRect rect;
		GetDlgItem(IDC_RawImgRect)->GetWindowRect(&rect);
		ScreenToClient(&rect);

		IMG colorIMG = m_firstProcess.GetRawImage();
		DispCVBImg (colorIMG, hdc, rect);
	}

	{
		CRect rect;
		GetDlgItem(IDC_RawImgRect2)->GetWindowRect(&rect);
		ScreenToClient(&rect);

		IMG monoIMG = m_secondProcess.GetRawImage();
		DispCVBImg (monoIMG, hdc, rect);
	}

	if(m_cvbHdlDisp)
	{
		CRect rect;
		GetDlgItem(IDC_HdlImgRect)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		DispCVBImg(	m_cvbHdlDisp, hdc,	rect);
	}
}
/*******************************************************************************
*	�������ƣ�	DispCVBImg
*	
*	����	��IMG 	HDC		RECT
*			  
*			  
*	���	����
*
*	����	����ʾͼ��
*
*
********************************************************************************/

void CapsuleSorterDlg::DispCVBImg(IMG imgIn,
								  HDC hdc,
								  RECT &rect)
{
	if(IsImage(imgIn))
	{
		long	width	= ImageWidth	(imgIn);
		long	height	= ImageHeight	(imgIn);
		long	dim		= ImageDimension(imgIn);
		long	scale	= width <=256 ? 1 : 3;

		long	redPage		= 0;
		long	greenPage	= 0;
		long	bluePage	= 0;

		if( 3 == dim)
		{
			redPage		= 0;
			greenPage	= 1;
			bluePage	= 2;
		}

		ImageToDC (	imgIn, hdc, rect.left, rect.top,
					0, 0, width-1,  height-1,
					rect.right - rect.left, rect.bottom - rect.top,
					redPage, greenPage, bluePage, 1.0, 0); 
	}
}

void CapsuleSorterDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	OnFreezeImg();
	m_beaconThread.Stop();
	CtrlCardComm::TheComm().Close();

	ConfigInfo& cfgInfo = ConfigInfo::TheConfigInfo();

	if (m_solution_X != cfgInfo.Solution_X() || m_solution_Y != cfgInfo.Solution_Y())
	{
		ChangeSolution(m_solution_X, m_solution_Y );
	}
}
/*******************************************************************************
*	�������ƣ�	OnClearCount
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	����������
*
*
********************************************************************************/
void CapsuleSorterDlg::OnClearCount() 
{
	CapsuleProc::ClearCount();
	m_allCount	= CapsuleProc::AllCount();
	m_badCount	= CapsuleProc::BadCount();
	m_secondProcess.ClearRecordedImage();
	m_firstProcess.ClearRecordedImage();
	UpdateData(FALSE);
}

void CapsuleSorterDlg::OnChangeTestSpec() 
{
	size_t observingTarget = GetDlgItemInt(IDC_TestTarget);
	CapsuleProc::ObservingTarget(observingTarget);
}

/*******************************************************************************
*	�������ƣ�	OnLoadImage
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	������ͼ��
*
*
********************************************************************************/
void CapsuleSorterDlg::OnLoadImage() 
{
	m_secondCam.StopLive();
	m_firstCam.StopLive();

	PostMessage(WM_COMMAND, IDC_SOFTKEYBD);
	
	CFileDialog dlg( TRUE,						//file open
		_T("bmp"),								//file ext
		_T("*.bmp"),							//file name
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,	// flags
		_T("Project file (*.bmp)|*.bmp|"));		// filter
	
	if(IDOK == dlg.DoModal())
	{
		CString fileName = dlg.GetPathName();
		TImgArchive bmpFile;
		if(bmpFile.Load(LPCTSTR(fileName)))
		{
			TAlloc<PixelMem> mem = bmpFile.GetImgMem();
			TImgDim			 dim = bmpFile.Dimension();
			if( 3 == dim.bytesPerPixel)
			{
				TImage<PelRGB24> img(mem, dim);
				TImgProcess::SwapBmpRB(img, img);
			}
			PixelMem *	pExchange	= new PixelMem[dim.bytesPerRow];
			size_t		halfHeight	= dim.height / 2;
			TBmpInfo	bmpInfo		= bmpFile.GetBmpInfo();			
			PixelMem	*pFileStart	= bmpFile.GetImgMem().Base();
			PixelMem	*pFileEnd	= pFileStart + dim.width * (dim.height - 1) * dim.bytesPerPixel;
			for(size_t XIndex = 0; XIndex < halfHeight; XIndex++)
			{
				memcpy(pExchange,	pFileStart, dim.bytesPerRow);
				memcpy(pFileStart,	pFileEnd, dim.bytesPerRow);
				memcpy(pFileEnd,	pExchange, dim.bytesPerRow);
				pFileStart	+= dim.bytesPerRow;
				pFileEnd	-= dim.bytesPerRow;
			}
			TImgBuffer	imgBuff(mem, dim, 1);
			
			//m_simCam.SetRawImage(imgBuff);
			if (-1 == fileName.Find("Color"))
			{
				m_secondSimCam.SetRawImage(imgBuff);
				m_secondSimCam.StartLive();
			}
			else
			{
				m_simCam.SetRawImage(imgBuff);
				m_simCam.StartLive();
			}
			
			m_secondProcess.ReInit	(dim);
			m_firstProcess.ReInit	(dim);
		}
		else
		{
			MessageBox("BMP file load false! \nThen load the deafault BMP!");
		}
	}
}
/*******************************************************************************
*	�������ƣ�	OnSaveImage
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	������ͼ��
*
*
********************************************************************************/
void CapsuleSorterDlg::OnSaveImage() 
{
	CFileDialog dlg( FALSE,						//file save
		_T("bmp"),								//file ext
		_T("*.bmp"),							//file name
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,	// flags
		_T("Project file (*.bmp)|*.bmp|"));		// filter

	PostMessage(WM_COMMAND, IDC_SOFTKEYBD);
	
	if(IDOK == dlg.DoModal())
	{
		CString fileName = dlg.GetPathName();
		int pos = fileName.Find(".bmp",0);

		CString colorFileName = fileName;
		colorFileName.Insert(pos, "Color");
		IMG colorIMG = m_firstProcess.GetRawImage( );
		WriteImageFile(colorIMG, (PCHAR)(LPCTSTR)colorFileName);

		CString monoFileName = fileName;
		monoFileName.Insert(pos, "Mono");
		IMG monoIMG = m_secondProcess.GetRawImage( );
		WriteImageFile(monoIMG, (PCHAR)(LPCTSTR)monoFileName);

	}	
}
/*******************************************************************************
*	�������ƣ�	OnSaveMultiImage
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	��������ͼ��
*
*
********************************************************************************/
void CapsuleSorterDlg::OnSaveMultiImage() 
{
	CString		fileName;
	CPathSelect dlg;
	if(IDC_BTN_DEF == dlg.DoModal())
	{
		fileName = TWinUtility::GetExeFileDir().c_str();
		fileName += "\\Template.mfd";
	}
	else
	{
		PostMessage(WM_COMMAND, IDC_SOFTKEYBD);
		CFileDialog fDlg(	FALSE,						//file save
							_T("mfd"),								//file ext
							_T("*.mfd"),							//file name
							OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,	// flags
							_T("Multi Frame file (*.mfd)|*.mfd|"));	// filter
		fDlg.m_ofn.lpstrInitialDir = GetExeFileDir().c_str();
		if (IDOK == fDlg.DoModal())
		{
			fileName = fDlg.GetPathName();
		}				
	}
	
	if(fileName.IsEmpty())	return;

	int pos = fileName.Find(".mfd",0);
	TMfdArchive mfdFile;	
		
	CString colorFileName = fileName;
	colorFileName.Insert(pos, "Color");
	mfdFile.SetMultiImage(m_firstProcess.GetRecordedImage());
	mfdFile.Save(LPCTSTR(colorFileName));
		
	CString monoFileName = fileName;
	monoFileName.Insert(pos, "Mono");
	mfdFile.SetMultiImage(m_secondProcess.GetRecordedImage());
	mfdFile.Save(LPCTSTR(monoFileName));
}
/*******************************************************************************
*	�������ƣ�	OnLoadMultiImage
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	��������ͼ��
*
*
********************************************************************************/
void CapsuleSorterDlg::OnLoadMultiImage() 
{
	m_secondCam.StopLive();
	m_firstCam.StopLive();

	PostMessage(WM_COMMAND, IDC_SOFTKEYBD);

	CFileDialog dlg( TRUE,									//file open
		_T("mfd"),											//file ext
		_T("*.mfd"),										//file name
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,				// flags
		_T("Multi Frame file (*.mfd)|*.mfd|"));				// filter
	
	dlg.m_ofn.lpstrInitialDir = GetExeFileDir().c_str();
	
	if(IDOK == dlg.DoModal())
	{
		CString fileName = dlg.GetPathName();
		TMfdArchive mfdFile;
		for(int i = 0; i < THREADCOUNT; i++)
		{
			//m_firstThread[i].SetThreadFunc	(	CapsuleSorterDlg::FirstCamFunc, this);
			m_firstThread[i].Start(TThread::HIGHEST);
			//m_secondThread[i].SetThreadFunc(	CapsuleSorterDlg::SecondCamFunc, this);		
			m_secondThread[i].Start(TThread::HIGHEST);
		}
		if(mfdFile.Load(LPCTSTR(fileName)))
		{
			TImgBuffer buff = mfdFile.GetImgBuffer();
			if (-1 == fileName.Find("Color"))
			{
				m_secondSimCam.SetRawImage(buff);
				m_secondSimCam.StartLive();

			}
			else
			{
				m_simCam.SetRawImage(buff);
				m_simCam.StartLive();
			}			
		}
	}
	
	TImgDim	dim = m_simCam.ImageDimension();
	m_firstProcess.ReInit(dim);
	dim = m_secondSimCam.ImageDimension();
	m_secondProcess.ReInit(dim);
}
/*******************************************************************************
*	�������ƣ�	OnIsSim
*	
*	����	����	
*			  
*			  
*	���	����
*
*	����	���жϹ����ں���ģʽ�£�����ʼ����Ӧ�Ĺ���
*
*
********************************************************************************/
void CapsuleSorterDlg::OnIsSim() 
{
	UpdateData(TRUE);
	TImgDim	colorImgDim, monoImgDim;
	const size_t frameCount = 30;
	if(m_isSim)
	{
		m_firstCam.StopLive();
		m_secondCam.StopLive();

		TImgDim dim = m_simCam.ImageDimension();		
		m_firstProcess.ReInit(dim);		
		dim = m_secondSimCam.ImageDimension();
		m_secondProcess.ReInit (dim);	
	}
	else
	{
		m_simCam.StopLive();
		m_secondSimCam.StopLive();

		m_firstCam.Open();
		TImgDim dimColor = m_firstCam.ImageDimension();
		m_firstProcess.ReInit(dimColor);

		m_secondCam.Open();
		TImgDim dimMono  = m_secondCam.ImageDimension();
		m_secondProcess.ReInit(dimMono);
	}
}

HBRUSH CapsuleSorterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	static HBITMAP bmp = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1));
	static HBRUSH brush = CreatePatternBrush(bmp);
	
	
	switch(nCtlColor)   
	{   
		
	case   CTLCOLOR_STATIC:   
		{
			pDC->SetTextColor(RGB(255,255,0));   
			pDC->SetBkMode(TRANSPARENT); 						
			return   (HBRUSH)   (brush);  
		}
		
	case CTLCOLOR_EDIT : //�ڴ˼�������Ҫ�ı䱳��ɫ�Ŀؼ���Ϣ 
		{
			static HBRUSH B = CreateSolidBrush(YELLOW ); //COLOR���������õ���ɫ 
			pDC->SetBkMode(TRANSPARENT  );  
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkColor(YELLOW);
			return (HBRUSH) B; 
		}
	case CTLCOLOR_MSGBOX:
		{
			static HBRUSH B = CreateSolidBrush(YELLOW);
			return (HBRUSH) B ;
		}
		
	case   CTLCOLOR_DLG:   
		return   (HBRUSH)   (brush); 

	default:   
		return   hbr; 
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CapsuleSorterDlg::OnSoftcfg() 
{
	// TODO: Add your control notification handler code here
	//CSelectMode mode;
	CSoftWareConfig	sfCfg;
	sfCfg.DoModal();
}

void CapsuleSorterDlg::OnHardcfg() 
{
	// TODO: Add your control notification handler code here
	CHardWareConfig hdCfg;
	hdCfg.m_transparent		 = m_firstProcess.SegmentParam().transparent?TRUE:FALSE;

	hdCfg.DoModal();

	SortorParam param = m_firstProcess.SegmentParam();
	param.transparent = hdCfg.m_transparent ? 1 : 0;
	m_firstProcess.SegmentParam(param);

}

void CapsuleSorterDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CtrlCardComm::TheComm().LightCtrl(5, true);
	CDialog::OnClose();

	
}

void CapsuleSorterDlg::OnOK() 
{
	// The Cameras Stop Capturing
	try
	{
		m_firstCam.StopLive();
		m_secondCam.StopLive();

		//Vertify to Exit
		CVertifyDlg dlg;
		dlg.SetInformation("ȷ���˳���");
		if(IDOK != dlg.DoModal()) {	return;	}
		CtrlCardComm::TheComm().LightCtrl(5, true);

		//Save camera Config file
		if(m_firstCam.IsOpen() && m_secondCam.IsOpen())
		{
			m_firstCam.SaveSetting();
			m_secondCam.SaveSetting();	
		}
		
		m_firstCam.Close();
		m_secondCam.Close();
	}
	catch(...)
	{

	}
	CDialog::OnOK();
}
/*******************************************************************************
*	�������ƣ�	UpdateConfig
*	
*	����	��	TString		����������ļ�������		
				EnumPath	���·��ѡ��
*			  
*			  
*	���	��	�������������Ƿ�ɹ�
*
*	����	��	���³������õ�����������
*
********************************************************************************/

bool CapsuleSorterDlg::UpdateConfig(const TString& iniFile, TIniFile::EnumPath ePath)
{
	ConfigInfo& cfgInfo	= ConfigInfo::TheConfigInfo();
	bool success	= cfgInfo.LoadIniFile(iniFile, ePath);
	if(success)
	{	
        m_firstProcess.CapsuleInfo	(	cfgInfo.FirstCapsuleInfo()			);
		m_secondProcess.CapsuleInfo	(	cfgInfo.SecondCapsuleInfo()			);
		m_firstProcess.SegmentParam	(	cfgInfo.ColorInfo()					);
		m_firstProcess.RemainInfo	(	cfgInfo.RemainInfo(ConfigInfo::FIRST));
		m_secondProcess.SegmentParam(	cfgInfo.MonoInfo()					);
		m_secondProcess.RemainInfo	(	cfgInfo.RemainInfo(ConfigInfo::SECOND));

		Valve::TheValve().SetParam	(	cfgInfo.ValveInfo() );
		CtrlCardComm::TheComm().SetParam(cfgInfo.ComCtrlInfo());
	}
	return success;
}

/*******************************************************************************
*	�������ƣ�	ConfigInterFace
*	
*	����	��	��
*			  
*			  
*	���	��	��
*
*	����	��	���½�����ʾ
*
********************************************************************************/

void CapsuleSorterDlg::ConfigInterFace()
{
	VERIFY(m_colorbutton[0].Attach(IDC_ClearCount,		this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[1].Attach(IDC_SaveMultImg,		this, PURPLE, WHITE, DKBLUE));
	
	VERIFY(m_colorbutton[2].Attach(IDC_GrabImage,		this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[3].Attach(IDC_FreezeImg,		this, PURPLE, WHITE, DKBLUE));
	
	VERIFY(m_colorbutton[4].Attach(IDC_SOFTCFG,			this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[5].Attach(IDC_HARDCFG,			this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[6].Attach(IDC_LoadConfigFile,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[7].Attach(IDC_SaveConfigFile,	this, PURPLE, WHITE, DKBLUE));
	
	VERIFY(m_colorbutton[8].Attach (IDC_LoadImage,		this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[9].Attach (IDC_SaveImage,		this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[10].Attach(IDC_LoadMultImg,	this, PURPLE, WHITE, DKBLUE));
	
	VERIFY(m_colorbutton[11].Attach(IDC_SOFTKEYBD,		this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[12].Attach(IDOK,				this, PURPLE, WHITE, DKBLUE));
	
	VERIFY(m_colorbutton[13].Attach(IDC_START_COMTEST,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[14].Attach(IDC_END_COMTEST,	this, PURPLE, WHITE, DKBLUE));

	bool simulator = ConfigInfo::TheConfigInfo().Simulation();

	if(simulator)
	{
		::ShowWindow(GetDlgItem(IDC_IsSim)->m_hWnd,			SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_LoadImage)->m_hWnd,		SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_SaveImage)->m_hWnd,		SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_LoadMultImg)->m_hWnd,	SW_SHOW);
		
		::ShowWindow(GetDlgItem(IDC_TestGroup)->m_hWnd,		SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_TestTarget)->m_hWnd,	SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_TestResult)->m_hWnd,	SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_TargetTitle)->m_hWnd,	SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_ResultTitle)->m_hWnd,	SW_SHOW);	
		::ShowWindow(GetDlgItem(IDC_START_COMTEST)->m_hWnd,	SW_SHOW);
		::ShowWindow(GetDlgItem(IDC_END_COMTEST)->m_hWnd,	SW_SHOW);
	}
	else
	{
		::ShowWindow(GetDlgItem(IDC_IsSim)->m_hWnd,		SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_LoadImage)->m_hWnd,		SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_SaveImage)->m_hWnd,		SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_LoadMultImg)->m_hWnd,	SW_HIDE);
		
		::ShowWindow(GetDlgItem(IDC_TestGroup)->m_hWnd,		SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_TestTarget)->m_hWnd,	SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_TestResult)->m_hWnd,	SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_TargetTitle)->m_hWnd,	SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_ResultTitle)->m_hWnd,	SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_START_COMTEST)->m_hWnd,	SW_HIDE);
		::ShowWindow(GetDlgItem(IDC_END_COMTEST)->m_hWnd,	SW_HIDE);
	}
}

/*******************************************************************************
*	�������ƣ�	GetCamera
*	
*	����	��	FirstCam	ָ���һ�������ָ��
				SecondCam	ָ��ڶ��������ָ��
*			  
*			  
*	���	��	��
*
*	����	��	������ṩ��������Ľӿ�
*
********************************************************************************/

void CapsuleSorterDlg::GetCamera(FirstCam* &pProCam, SecondCam* &pGenCam)
{
	pProCam = &m_firstCam;
	pGenCam = &m_secondCam;
}

/*******************************************************************************
*	�������ƣ�	GetProcessor
*	
*	����	��	TImgDim		ͼ���ʽ��Ϣ
*			  
*			  
*	���	��	����ģ��ָ��
*
*	����	��	������ṩ���Ӧͼ��Ĵ���ģ��ӿ�
*
********************************************************************************/

CapsuleProc* CapsuleSorterDlg::GetProcessor(const TImgDim& imgDim)
{
	if(3 == imgDim.bytesPerPixel)
	{
		m_firstCam.StopLive();
		m_simCam.StopLive();	
		m_firstProcess.ReInit(imgDim);
		return &m_firstProcess;
	}
	if(1 == imgDim.bytesPerPixel)
	{
		m_secondCam.StopLive();
		m_simCam.StopLive();
		m_secondProcess.ReInit(imgDim);
		return &m_secondProcess;
	}
	return 0;
}
/*******************************************************************************
*	�������ƣ�	GetProcessor
*	
*	����	��	size_t		ͼ���ʽ��Ϣ
*			  
*			  
*	���	��	����ģ��ָ��
*
*	����	��	������ṩ���Ӧͼ��Ĵ���ģ��ӿ�
*
********************************************************************************/

CapsuleProc* CapsuleSorterDlg::GetProcessor(size_t colorChannelCount)
{
	if		(3 == colorChannelCount)	{	return &m_firstProcess;	}
	else if	(1 == colorChannelCount)	{	return &m_secondProcess;}
	else	{	return 0;		}
}

void CapsuleSorterDlg::OnSoftKeyBD() 
{
	CKeyBoardDlg::DisplayWindow(FALSE != m_isSim);
}

void CapsuleSorterDlg::OnLoadConfigFile() 
{
	CFileDialog dlg( TRUE,									//file open
			_T("ini"),								//file ext
			NULL,							//file name
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,	// flags
			_T("Project file (*.ini)|*.ini|"));		// filter
	dlg.m_ofn.lpstrInitialDir = GetExeFileDir().c_str();
	PostMessage(WM_COMMAND, IDC_SOFTKEYBD);
	if(IDOK == dlg.DoModal())
	{
		CString fileName = dlg.GetPathName();
		UpdateConfig(LPCTSTR(fileName),TIniFile::eOther);		
	}
}

void CapsuleSorterDlg::OnSaveConfigFile() 
{
	CVertifyDlg dlg;
	dlg.SetInformation("ȷ���޸ģ�");
	if(IDOK != dlg.DoModal())  return;	

	PostMessage(WM_COMMAND, IDC_SOFTKEYBD);

	ConfigInfo& cfgInfo = ConfigInfo::TheConfigInfo();
	cfgInfo.FirstCapsuleInfo(m_firstProcess.CapsuleInfo());
	cfgInfo.SecondCapsuleInfo(m_secondProcess.CapsuleInfo());

	cfgInfo.MonoInfo(m_secondProcess.SegmentParam());
	cfgInfo.ColorInfo(m_firstProcess.SegmentParam());
	cfgInfo.ValveInfo(Valve::TheValve().GetParam());
	cfgInfo.RemainInfo(ConfigInfo::FIRST, m_firstProcess.RemainInfo());
	cfgInfo.RemainInfo(ConfigInfo::SECOND, m_secondProcess.RemainInfo());

	CameraParam camParam = cfgInfo.SecondCamInfo();;
	if(m_secondCam.IsOpen())
	{
		m_secondCam.GetROI	(	camParam.roiStartX,
						camParam.roiStartY,
						camParam.roiWidth,
						camParam.roiHeight);
		m_secondCam.GetGain	(	camParam.gain);
		m_secondCam.GetExpTime	(	camParam.expTime);
	}
	cfgInfo.SecondCamInfo(camParam);
	
	camParam = cfgInfo.FirstCamInfo();
	if(m_firstCam.IsOpen())
	{
		m_firstCam.GetROI	(	camParam.roiStartX,
						camParam.roiStartY,
						camParam.roiWidth,
						camParam.roiHeight);
		m_firstCam.GetGain	(	camParam.gain);
		m_firstCam.GetExpTime	(	camParam.expTime);
		m_firstCam.GetWhiteBal	(	camParam.balanceRed,
						camParam.balanceGreen,
						camParam.balanceBlue);
	}
	cfgInfo.FirstCamInfo(camParam);

	cfgInfo.SaveIniFile("Capsule.ini", TIniFile::eExeDir);	
	
	CFileDialog fDlg( FALSE,					//file open
		_T("ini"),								//file ext
		NULL,									//file name
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,	// flags
		_T("Project file (*.ini)|*.ini|"));		// filter

	if (IDOK == fDlg.DoModal())
	{
		CString bakeSeries = fDlg.m_ofn.lpstrFileTitle;
		bakeSeries.Insert(0, "Bak");
		cfgInfo.SaveIniFile(LPCTSTR(bakeSeries), TIniFile::eExeDir);			
	}		
	
}
/*******************************************************************************
*	�������ƣ�	IsBadHigh
*	
*	����	��	��			  
*			  
*	���	��	��
*
*	����	��	�����Ƶı���������	
*
*
********************************************************************************/

void	CapsuleSorterDlg::IsBadHigh		()
{
	const double	badLimit = 0.2;
	if (m_badCount > (m_allCount* badLimit +1.0) && m_allCount > 100)
	{
		TBeacon::TheBeacon().Alarm();
	}
}
/*******************************************************************************
*	�������ƣ�	GetExeFileDir
*	
*	����	��				  
*			  
*	���	��	string	������ִ���ļ����ļ���·��
*
*	����	��	�õ���ִ���ļ����ļ���·��
*
*
********************************************************************************/

std::string&	CapsuleSorterDlg::GetExeFileDir	()
{
	static	std::string		exeFileDir;
	if (!exeFileDir.size())
	{
		char	filePath[MAX_PATH];
		GetModuleFileName(NULL, filePath, MAX_PATH);
		exeFileDir = filePath;
		size_t pathDot = exeFileDir.rfind("\\") + 1;
		exeFileDir = exeFileDir.substr(0, pathDot);
	}
	return exeFileDir;
}

void CapsuleSorterDlg::OnStartComtest() 
{
	// TODO: Add your control notification handler code here
	CtrlCardComm &comctrl = CtrlCardComm::TheComm();
	comctrl.ClearRxBuffer();
}

void CapsuleSorterDlg::OnEndComtest() 
{
	// TODO: Add your control notification handler code here
	CtrlCardComm	&comctrl = CtrlCardComm::TheComm();
	size_t			buffSize = 0;
	CtrlCardComm::COMMCODE			*pBuffer = reinterpret_cast<CtrlCardComm::COMMCODE*>(comctrl.GetRxBuffer(buffSize));
	const CtrlCardComm::COMMCODE	cmdRx(0XA0, 0XFF, 0X01, 0XAF);
	const CtrlCardComm::COMMCODE	excRx(0XA0, 0X05, 0X01, 0XAF);
	size_t			repeatT		= buffSize /4;
	size_t			cmdRxCount	= 0;
	size_t			excCount	= 0;
	for(size_t i = 0; i < repeatT; i++)
	{
		if (pBuffer->code == cmdRx.code)
		{
			++cmdRxCount;
		}
		else if (pBuffer->code == excRx.code)
		{			
			++excCount;
		}
		pBuffer++;
	}	
	CString text;
	text.Format("%d�ν������� \n%d��ִ��", cmdRxCount, excCount);
	MessageBox(text);
}

/*******************************************************************************
*	�������ƣ�	FirstCamFunc
*	
*	����	��	
*			  
*	���	��	��
*
*	����	��	��һ������Ĵ�����
*
*
********************************************************************************/

void	CapsuleSorterDlg::FirstCamFunc	(void *param)
{
	CapsuleSorterDlg *pThis = reinterpret_cast<CapsuleSorterDlg*>(param);
	
	TImageCard	*pCamCard = pThis->m_isSim	?reinterpret_cast<TImageCard*>(&pThis->m_simCam)
											:reinterpret_cast<TImageCard*>(&pThis->m_firstCam);
	while (1)
	{		
		pCamCard->Wait();
		pThis->m_firstSect.Lock();
		Valve::EIntervalType interType = Valve::eShortInterval;		
		try
		{
			size_t period	= pCamCard->FrameInterval();
			static size_t lastPeriod = period;
			interType = lastPeriod > period ? Valve::eShortInterval:Valve::eLongInterval;
			lastPeriod = period;
			size_t badCount = 0;
			if (Valve::eShortInterval == interType)
			{
				badCount	= Valve::TheValve().CtrlResult(	Valve::eFirst);
			}	
			CapsuleProc::AddToBadCount(badCount);
		}
		catch(...)
		{
			FILE* pFile = NULL;
			fopen_s(&pFile, "D:\\logperiod.txt", "wb");
			fwrite("period error", sizeof(char), strlen("period error"), pFile);
			fclose(pFile);
		}
		
		void*	pMem	=	pCamCard->GetPixelMemBase();
		size_t	bytes	=	pCamCard->Width()
			* pCamCard->Height() 
			* pCamCard->BytesPerPixel();
		
		if(pThis->m_firstProcess.SetRawImage(pMem, bytes))
		{
			unsigned int result = 0;
			try
			{
				result		= pThis->m_firstProcess.Process( );
			}
			catch(...)
			{
				FILE* pFile = NULL;
				fopen_s(&pFile, "D:\\logprocess.txt", "wb");
				fwrite("process error", sizeof(char), strlen("process error"), pFile);
				fclose(pFile);
			}			
			try
			{
				size_t		ctlResult	= result &0x0f;		
				if (Valve::eShortInterval == interType)
				{
					Valve::TheValve().ShortInterval(Valve::eFirst, ctlResult);
				}
				else
				{
					Valve::TheValve().LongInterval(Valve::eFirst, ctlResult);
				}
			}
			catch(...)
			{
				FILE* pFile = NULL;
				fopen_s(&pFile, "D:\\logTheValve.txt", "wb");
				fwrite("TheValve error", sizeof(char), strlen("TheValve error"), pFile);
				fclose(pFile);
			}				
		}
		try
		{		
			pThis->m_firstSect.Unlock();
			pThis->m_cvbHdlDisp	= CapsuleProc::ObservedIMG();
			pThis->m_testResult	= CapsuleProc::ObservedParam();
			pThis->m_badCount	= CapsuleProc::BadCount();
			
			CRect rect;
			pThis->GetDlgItem(IDC_RawImgRect)->GetWindowRect(&rect);
			pThis->InvalidateRect(&rect, FALSE);
			pThis->GetDlgItem(IDC_HdlImgRect)->GetWindowRect(&rect);
			pThis->InvalidateRect(&rect, FALSE);
			pThis->UpdateData(FALSE);
		}
		catch(...)
		{
		}		
	}
	
}

/*******************************************************************************
*	�������ƣ�	SecondCamFunc
*	
*	����	��	
*			  
*	���	��	��
*
*	����	��	�ڶ�������Ĵ�����
*
*
********************************************************************************/

void	CapsuleSorterDlg::SecondCamFunc	(void *param)
{
	CapsuleSorterDlg*	pThis = (CapsuleSorterDlg*)(param);
	
	TImageCard	*pCamCard = pThis->m_isSim	?reinterpret_cast<TImageCard*>(&pThis->m_secondSimCam)
		:reinterpret_cast<TImageCard*>(&pThis->m_secondCam);
	while(1)
	{
		pCamCard->Wait();
		pThis->m_secondSect.Lock();
		
		Valve::EIntervalType interType = Valve::eShortInterval;
		try
		{
			size_t period = pCamCard->FrameInterval();
			static size_t lastPeriod =  period;
			size_t badCount = 0;
			interType = lastPeriod > period ? Valve::eShortInterval:Valve::eLongInterval;
			lastPeriod = period;
			if (Valve::eShortInterval == interType)
			{
				badCount	= Valve::TheValve().PushResult(Valve::eSecond);
			}
		}
		catch(...)
		{
			FILE* pFile = NULL;
			fopen_s(&pFile, "D:\\logperiod2.txt", "wb");
			fwrite("period error", sizeof(char), strlen("period error"), pFile);
			fclose(pFile);
		}
		
		void*	pMem	=	pCamCard->GetPixelMemBase();
		size_t	bytes	=	pCamCard->Width() * pCamCard->Height() * pCamCard->BytesPerPixel();	
		
		if(pThis->m_secondProcess.SetRawImage(pMem, bytes))
		{	
			unsigned int	result = 0;
			try
			{
				result	= pThis->m_secondProcess.Process( );
			}
			catch(...)
			{
				FILE* pFile = NULL;
				fopen_s(&pFile, "D:\\logprocess2.txt", "wb");
				fwrite("process error", sizeof(char), strlen("process error"), pFile);
				fclose(pFile);
			}
			
			try
			{
				size_t		ctlResult	= result &0x0f;
				if (Valve::eShortInterval == interType)
				{
					Valve::TheValve().ShortInterval(Valve::eSecond, ctlResult);
				}
				else
				{
					Valve::TheValve().LongInterval(Valve::eSecond, ctlResult);
				}	
			}
			catch(...)
			{
				FILE* pFile = NULL;
				fopen_s(&pFile, "D:\\logTheValve2.txt", "wb");
				fwrite("TheValve error", sizeof(char), strlen("TheValve error"), pFile);
				fclose(pFile);
			}
		}	
		try
		{
			pThis->m_secondSect.Unlock();
			pThis->m_cvbHdlDisp	= CapsuleProc::ObservedIMG();
			pThis->m_testResult	= CapsuleProc::ObservedParam();	
			pThis->m_allCount	= CapsuleProc::AllCount() ;
			
			CRect rect;
			pThis->GetDlgItem(IDC_RawImgRect2)->GetWindowRect(&rect);
			pThis->InvalidateRect(&rect, FALSE);
			pThis->GetDlgItem(IDC_HdlImgRect)->GetWindowRect(&rect);
			pThis->InvalidateRect(&rect, FALSE);
			pThis->UpdateData(FALSE);
		}
		catch(...)
		{}
		
	}	
}

/*******************************************************************************
*	�������ƣ�	InitImageCard
*	
*	����	��	
*			  
*	���	��	��
*
*	����	��	��ʼ��ͼ��ɼ��豸.
*
*
********************************************************************************/
void	CapsuleSorterDlg::InitImageCard	()
{	
	m_simCam.Open();
	m_secondSimCam.Open();

	ConfigInfo&	info	=	ConfigInfo::TheConfigInfo();
	
	TImageCard	*pFirstCam	= &m_firstCam;
	TImageCard	*pSecondCam = &m_secondCam;
	bool camOpened		=	pFirstCam->Open(info.FirstCamInfo().camTag.c_str()) &&
							pSecondCam->Open(info.SecondCamInfo().camTag.c_str());

	CameraParam camParam	=	info.SecondCamInfo();	
	if(m_secondCam.IsOpen())
	{		
		if (!m_secondCam.SetROI(camParam.roiStartX,	camParam.roiStartY,	camParam.roiWidth,	camParam.roiHeight))
 		{
			AfxMessageBox("�������ROIʧ�ܣ�");
 		}
 				
		m_secondCam.SetGain(camParam.gain);
		m_secondCam.SetExpTime(camParam.expTime);
	}
	
	camParam= info.FirstCamInfo();	
	if(m_firstCam.IsOpen())
	{		
		if (!m_firstCam.SetROI(  camParam.roiStartX,	camParam.roiStartY,	camParam.roiWidth,	camParam.roiHeight))
		{
			AfxMessageBox("�������ROIʧ��!");
		}		
 				
		m_firstCam.SetGain(	camParam.gain);
		
		m_firstCam.SetExpTime(	camParam.expTime);		
	}
	
	m_simCam.RegisterProcFunc	(	CapsuleSorterDlg::FirstCamFunc,		this);
	m_secondSimCam.RegisterProcFunc	(	CapsuleSorterDlg::SecondCamFunc,	this);

//	m_firstManager.SetThreadFunc(CapsuleSorterDlg::FirstManagerFunc, this);
//	m_secondManager.SetThreadFunc(CapsuleSorterDlg::SecondManagerFunc, this);

	for(int i = 0; i < THREADCOUNT; i++)
	{
		m_firstThread[i].SetThreadFunc	(	CapsuleSorterDlg::FirstCamFunc, this);		
		m_secondThread[i].SetThreadFunc(	CapsuleSorterDlg::SecondCamFunc, this);
		if (!info.WorkMode())
		{
			m_firstThread[i].Start(TThread::HIGHEST);	
			m_secondThread[i].Start(TThread::HIGHEST);
		}
		
	}
	
	
	if(camOpened)
	{
		TImgDim		firstDim	=	m_firstCam.ImageDimension();
		m_firstProcess.ReInit(	firstDim);
		
		TImgDim		secondDim	=	m_secondCam.ImageDimension();
		m_secondProcess.ReInit(	secondDim);
	}
	else
	{
		TImgDim		dim			=	m_simCam.ImageDimension();	
		m_secondProcess.ReInit(dim);
		dim.bitsPerPixel		=	sizeof(FirstCamPixel)*8;
		dim.bytesPerPixel		=	sizeof(FirstCamPixel);
		dim.bytesPerRow			=	sizeof(FirstCamPixel)*dim.width;
		m_firstProcess.ReInit(dim);
	}

	m_firstProcess.SetProcessIndex(Valve::eFirst);
	m_secondProcess.SetProcessIndex(Valve::eSecond);
}

/*******************************************************************************
*	�������ƣ�	InitComPort
*	
*	����	��	
*			  
*	���	��	��
*
*	����	��	��ʼ������
*
*
********************************************************************************/

bool	CapsuleSorterDlg::InitComPort()
{
	if (!CtrlCardComm::TheComm().Open())
	{
		AfxMessageBox("���ڴ�ʧ�ܣ�");
		
	};
	
	if (!CtrlCardComm::TheComm().GetCurState())
	{
		AfxMessageBox("����ͨ��ʧ�ܣ�");
		CtrlCardComm::TheComm().Close();
		return false;
	}

	m_beaconThread.SetThreadFunc(CapsuleSorterDlg::BeaconFunc, this);
	m_beaconThread.Start();
	return true;
}

void	CapsuleSorterDlg::OnCancel()
{
}


void	CapsuleSorterDlg::ChangeSolution(const int width, const int height)
{
	DEVMODE devMode;
	devMode.dmBitsPerPel = 32;
	devMode.dmPelsWidth = width;
	devMode.dmPelsHeight = height;
	devMode.dmSize			= sizeof(devMode);
	devMode.dmFields   =DM_PELSWIDTH|DM_BITSPERPEL|DM_PELSHEIGHT;
	ChangeDisplaySettings(&devMode,0 );
}