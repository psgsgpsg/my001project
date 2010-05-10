// VideoAnalysisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoAnalysis.h"
#include "VideoAnalysisDlg.h"
#include "TImageQuality.h"

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
// CVideoAnalysisDlg dialog

CVideoAnalysisDlg::CVideoAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoAnalysisDlg::IDD, pParent),
	m_rawImage(NULL)
{
	//{{AFX_DATA_INIT(CVideoAnalysisDlg)
	m_edt_snr = _T("");
	m_edt_lght = _T("");
	m_edt_gray = _T("");
	m_edt_gain = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoAnalysisDlg)
	DDX_Control(pDX, IDC_PRGC_GAIN, m_prgc_gain);
	DDX_Control(pDX, IDC_PRGC_GRAY, m_prgc_gray);
	DDX_Control(pDX, IDC_PRGC_LGHT, m_prgc_lght);
	DDX_Control(pDX, IDC_PRGC_SNR, m_prgc_snr);
	DDX_Text(pDX, IDC_EDT_SNR, m_edt_snr);
	DDX_Text(pDX, IDC_EDT_LGHT, m_edt_lght);
	DDX_Text(pDX, IDC_EDT_GRAY, m_edt_gray);
	DDX_Text(pDX, IDC_EDT_GAIN, m_edt_gain);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVideoAnalysisDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoAnalysisDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOADIMAGE, OnLoadimage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoAnalysisDlg message handlers

BOOL CVideoAnalysisDlg::OnInitDialog()
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
	m_prgc_snr.SetRange(0, 100);
	m_prgc_lght.SetRange(0, 255);
	m_prgc_gray.SetRange(0, 255);
	m_prgc_gain.SetRange(0, 100);
	m_prgc_snr.SetWindowText("%50");
	m_prgc_snr.SetPos(50);
	m_prgc_snr.SendMessage(PBM_SETBARCOLOR, 0, (LPARAM)RGB(0, 200, 0));
	m_prgc_lght.SetPos(100);
	m_prgc_gray.SetPos(50);
	m_prgc_gain.SetPos(30);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoAnalysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVideoAnalysisDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoAnalysisDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVideoAnalysisDlg::OnLoadimage() 
{
	// TODO: Add your control notification handler code here
	CFileDialog	fd(TRUE);
	if (fd.DoModal() == IDCANCEL)
	{
		return;
	}
	if (m_rawImage)
	{
		cvReleaseImage(&m_rawImage);
	}
	m_rawImage = cvLoadImage(fd.m_ofn.lpstrFile, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	int meanValue = TImageQuality::GetImageMeanValue(m_rawImage);
	SetGrayMean(meanValue);


	SetDisplayImage(m_rawImage);

}

void CVideoAnalysisDlg::SetDisplayImage(IplImage * pImage)
{
	static IplImage * dispImage = NULL;
	if (!pImage&&!dispImage)
	{
		return;
	}
	if (pImage)
	{
		dispImage = pImage;
	}
	cvFlip(dispImage, NULL, -1);
	const int imgWidth = dispImage->width;
	const int imgHeight = dispImage->height;
	BITMAPINFO	bitInfo;
	memset(&bitInfo, 0x00, sizeof(BITMAPINFO));
	bitInfo.bmiHeader.biBitCount = dispImage->depth*dispImage->nChannels;
	bitInfo.bmiHeader.biHeight = imgHeight;
	bitInfo.bmiHeader.biWidth = imgWidth;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biSizeImage = dispImage->imageSize;
	bitInfo.bmiHeader.biPlanes = 1;
	CDC * pDC = GetDlgItem(IDC_DISPRECT)->GetDC();
	RECT rect;
	GetDlgItem(IDC_DISPRECT)->GetClientRect(&rect);
	int err = StretchDIBits(pDC->m_hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					0, 0, imgWidth-1, imgHeight-1, dispImage->imageData, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
}

void CVideoAnalysisDlg::SetGrayMean(int mean)
{
	m_prgc_gray.SetPos(mean);
	m_edt_gray.Format("%d", mean);
	UpdateData(FALSE);
}
