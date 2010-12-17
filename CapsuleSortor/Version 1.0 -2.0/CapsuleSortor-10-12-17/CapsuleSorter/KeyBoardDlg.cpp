// KeyBoardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "capsulesorter.h"
#include "KeyBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardDlg dialog

CKeyBoardDlg* CKeyBoardDlg::m_dlg = 0;

CKeyBoardDlg::CKeyBoardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyBoardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyBoardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKeyBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyBoardDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyBoardDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyBoardDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ESC, OnBtnEsc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardDlg message handlers

BOOL CKeyBoardDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	HandleBtnMsg(wParam);
	return CDialog::OnCommand(wParam, lParam);
}

bool CKeyBoardDlg::SendKeyValue(unsigned char keyVal)
{
	static bool first = true;
	static CString tmplTitle, cameraTitle, ctrlCardTitle, saveCfgTitle;
	static CString openDlgTitle, saveDlgTitle;
	if(first)
	{
		tmplTitle.LoadString	(IDS_TMPLCFG_TITLE);
		cameraTitle.LoadString	(IDS_CAMERACFG_TITLE);
		ctrlCardTitle.LoadString(IDS_CTRLCARDCFG_TITLE);
		openDlgTitle.LoadString (IDS_OPENDLG_TITLE);
		saveDlgTitle.LoadString (IDS_SAVEDLG_TITLE);
		saveCfgTitle.LoadString (IDS_SAVECFGFILE_TITLE);
		first = false;
	}

	{
		CWnd* openDlgWnd	= FindWindow( NULL, openDlgTitle);
		CWnd* saveDlgWnd	= FindWindow( NULL, saveDlgTitle);
		CWnd* saveCfgWnd	= FindWindow( NULL, saveCfgTitle);
		CWnd* tmplWnd		= FindWindow( NULL, tmplTitle);
		CWnd* cameraWnd		= FindWindow( NULL, cameraTitle);
		CWnd* ctrlCardWnd	= FindWindow( NULL, ctrlCardTitle);
		
		if(openDlgWnd)
		{
			openDlgWnd->SetForegroundWindow( );
		}
		else if(saveDlgWnd)
		{
			saveDlgWnd->SetForegroundWindow( );
		}
		else if(saveCfgWnd)
		{
			saveCfgWnd->SetForegroundWindow( );
		}
		else if(tmplWnd)
		{
			tmplWnd->SetForegroundWindow( );
		}
		else if(cameraWnd)
		{
			cameraWnd->SetForegroundWindow( );
		}
		else if(ctrlCardWnd)
		{	
			ctrlCardWnd->SetForegroundWindow( );
		}
		else
		{	
			GetParent()->SetForegroundWindow( );
		}
	}
	Sleep(10);
	INPUT input[2];
	memset(input, 0, 2*sizeof(INPUT));
	input[0].type		= INPUT_KEYBOARD;
	input[0].ki.wVk		= keyVal;
	input[1].type		= INPUT_KEYBOARD;
	input[1].ki.wVk		= keyVal;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(2, input, sizeof(INPUT));
	return true;
}
bool CKeyBoardDlg::HandleBtnMsg(WPARAM wParam)
{
	bool			success = true;
	unsigned char	keyVal	= 0;
	switch(LOWORD(wParam))
	{
	case IDC_BTN_ZERO:
		keyVal = '0';
		break;
	case IDC_BTN_ONE:
		keyVal = '1';
		break;
	case IDC_BTN_TWO:
		keyVal = '2';
		break;
	case IDC_BTN_THREE:
		keyVal = '3';
		break;
	case IDC_BTN_FOUR:
		keyVal = '4';
		break;
	case IDC_BTN_FIVE:
		keyVal = '5';
		break;
	case IDC_BTN_SIX:
		keyVal = '6';
		break;
	case IDC_BTN_SEVEN:
		keyVal = '7';
		break;
	case IDC_BTN_EIGHT:
		keyVal = '8';
		break;
	case IDC_BTN_NINE:
		keyVal = '9';
		break;
	case IDC_BTN_DOT:
		keyVal = '.';
		break;
	case IDC_BTN_BACK:
		keyVal = VK_BACK;
		break;
	case IDC_BTN_LEFT:
		keyVal = VK_LEFT;
		break;
	case IDC_BTN_RIGHT:
		keyVal = VK_RIGHT;
		break;
	default:
		success = false;
		keyVal = 0;
	}
	if(success)	SendKeyValue(keyVal);
	return success;
}

void CKeyBoardDlg::OnClose() 
{
	ShowWindow(SW_HIDE);
	//CDialog::OnClose();
}

void CKeyBoardDlg::OnBtnEsc() 
{
	ShowWindow(SW_HIDE);
}

BOOL CKeyBoardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	VERIFY(m_colorbutton[0].Attach(IDC_BTN_ZERO,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[1].Attach(IDC_BTN_ONE,		this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[2].Attach(IDC_BTN_TWO,		this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[3].Attach(IDC_BTN_THREE,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[4].Attach(IDC_BTN_FOUR,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[5].Attach(IDC_BTN_FIVE,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[6].Attach(IDC_BTN_SIX,		this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[7].Attach(IDC_BTN_SEVEN,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[8].Attach(IDC_BTN_EIGHT,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[9].Attach(IDC_BTN_NINE,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[10].Attach(IDC_BTN_DOT,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[11].Attach(IDC_BTN_BACK,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[12].Attach(IDC_BTN_LEFT,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[13].Attach(IDC_BTN_RIGHT,	this, PURPLE, WHITE, DKBLUE));
	VERIFY(m_colorbutton[14].Attach(IDC_BTN_ESC,	this, PURPLE, WHITE, DKBLUE));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool  CKeyBoardDlg::DisplayWindow(bool isSimulate)
{
	if(0 == m_dlg)
	{
		m_dlg = new CKeyBoardDlg(AfxGetMainWnd());
		m_dlg->Create(IDD_KEYBOARD);
	}
	
	RECT	rect;
	m_dlg->GetWindowRect(&rect);
	int	scWidth	= GetSystemMetrics(SM_CXSCREEN);
	int	scHeight= GetSystemMetrics(SM_CYSCREEN);

	int width	= rect.right  - rect.left;
	int height	= rect.bottom - rect.top;
	int left	= scWidth	  - width;
	int top		= (isSimulate) ? 0 : scHeight-height;
	
	m_dlg->SetWindowPos(&wndTopMost, left, top, width, height, SWP_SHOWWINDOW);
	return (0 != m_dlg);
}
