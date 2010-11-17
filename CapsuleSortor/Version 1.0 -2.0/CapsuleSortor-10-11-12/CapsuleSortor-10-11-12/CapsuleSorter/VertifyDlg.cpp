// VertifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "capsulesorter.h"
#include "VertifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVertifyDlg dialog


CVertifyDlg::CVertifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVertifyDlg::IDD, pParent),
	  m_verityMsg("确定执行此操作?")
{
	//{{AFX_DATA_INIT(CVertifyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVertifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVertifyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVertifyDlg, CDialog)
	//{{AFX_MSG_MAP(CVertifyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVertifyDlg message handlers

void CVertifyDlg::SetInformation(const CString info)
{
	m_verityMsg = info;
}

BOOL CVertifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_VERTIFYMSG, m_verityMsg);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


