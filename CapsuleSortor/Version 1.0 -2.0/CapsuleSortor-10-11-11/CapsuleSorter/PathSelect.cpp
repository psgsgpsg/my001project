// PathSelect.cpp : implementation file
//

#include "stdafx.h"
#include "capsulesorter.h"
#include "PathSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathSelect dialog


CPathSelect::CPathSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CPathSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPathSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPathSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathSelect)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPathSelect, CDialog)
	//{{AFX_MSG_MAP(CPathSelect)
	ON_BN_CLICKED(IDC_BTN_DEF, OnBtnDef)
	ON_BN_CLICKED(IDC_BTN_SELECT, OnBtnSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathSelect message handlers

void CPathSelect::OnBtnDef() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDC_BTN_DEF);	
}

void CPathSelect::OnBtnSelect() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDC_BTN_SELECT);
}
