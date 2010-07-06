// SpecifyCapsualeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "capsulesorter.h"
#include "KeyBoardDlg.h"
#include "SpecifyCapsualeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecifyCapsualeDlg dialog


CSpecifyCapsualeDlg::CSpecifyCapsualeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpecifyCapsualeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecifyCapsualeDlg)
	m_specifyNum = "";
	//}}AFX_DATA_INIT
}


void CSpecifyCapsualeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecifyCapsualeDlg)
	DDX_Text(pDX, IDC_SPECIFY_CAPSUAL, m_specifyNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpecifyCapsualeDlg, CDialog)
	//{{AFX_MSG_MAP(CSpecifyCapsualeDlg)
	ON_EN_CHANGE(IDC_SPECIFY_CAPSUAL, OnChangeSpecifyCapsual)
	ON_BN_CLICKED(IDC_CfgSoftKeyBD, OnCfgSoftKeyBD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecifyCapsualeDlg message handlers

void CSpecifyCapsualeDlg::OnChangeSpecifyCapsual() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	
	// TODO: Add your control notification handler code here
	
}

CString	CSpecifyCapsualeDlg::GetSpecifyNum()
{
	return m_specifyNum;
}

void CSpecifyCapsualeDlg::OnCfgSoftKeyBD() 
{
	// TODO: Add your control notification handler code here
	CKeyBoardDlg::DisplayWindow();	
}
