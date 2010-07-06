// SelectHDCfg.cpp : implementation file
//

#include "stdafx.h"
#include "capsulesorter.h"
#include "SelectHDCfg.h"
#include "controlboardsetting.h"
#include "camconfigsetting.h"
#include "CapsuleProc.h"
#include "Valve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHardWareConfig dialog


CHardWareConfig::CHardWareConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CHardWareConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHardWareConfig)
	m_transparent = FALSE;
	//}}AFX_DATA_INIT
}


void CHardWareConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHardWareConfig)
	DDX_Check(pDX, IDC_TRANSPARENT, m_transparent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHardWareConfig, CDialog)
	//{{AFX_MSG_MAP(CHardWareConfig)
	ON_BN_CLICKED(IDC_CAM_CFG, OnCamCfg)
	ON_BN_CLICKED(IDC_CONTROLBOARD_CFG, OnControlboardCfg)
	ON_BN_CLICKED(IDC_ValveEnable, OnValveEnable)
	ON_BN_CLICKED(IDC_SaveAll, OnSaveAll)
	ON_BN_CLICKED(IDC_WrongSave, OnWrongSave)
	ON_BN_CLICKED(IDC_FrontLightEnable, OnFrontLightEnable)
	ON_BN_CLICKED(IDC_TRANSPARENT, OnTransparent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHardWareConfig message handlers

void CHardWareConfig::OnCamCfg() 
{
	// TODO: Add your control notification handler code here	
	CCamConfigSetting	camSetting;
	camSetting.DoModal();
}

void CHardWareConfig::OnControlboardCfg() 
{
	// TODO: Add your control notification handler code here
	CControlBoardSetting ctrlBoard;
	ctrlBoard.DoModal();
}

BOOL CHardWareConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton *valveBtn   = (CButton*)GetDlgItem(IDC_ValveEnable);
	int     isChecking  = Valve::TheValve().IsEnable() ? 1 : 0;
	valveBtn->SetCheck(isChecking);
	
	SetSaveButtonTitle();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHardWareConfig::OnValveEnable() 
{
	CButton *valveBtn = (CButton*)GetDlgItem(IDC_ValveEnable);
	if(valveBtn->GetCheck() == 1)
	{	Valve::TheValve().Enable(true);	}
	else
	{	Valve::TheValve().Enable(false);	}
}

void CHardWareConfig::SetSaveButtonTitle( )
{
	switch(CapsuleProc::GetSaveState())
	{
	case CapsuleProc::eNoSave:
		CheckDlgButton(IDC_SaveAll,     FALSE);
		CheckDlgButton(IDC_WrongSave,   FALSE);
		break;
	case CapsuleProc::eErrSave:
		CheckDlgButton(IDC_WrongSave, TRUE);
		CheckDlgButton(IDC_SaveAll,	FALSE);
		break;
	case CapsuleProc::eAllSave:
		CheckDlgButton(IDC_SaveAll,	TRUE);
		CheckDlgButton(IDC_WrongSave,	FALSE);
		break;
	default:
		;
	}
}

void CHardWareConfig::OnSaveAll() 
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_SaveAll))->GetCheck())
	{
		CapsuleProc::SetSaveState(CapsuleProc::eAllSave);
		SetSaveButtonTitle();
	}
	else
	{
		CapsuleProc::SetSaveState(CapsuleProc::eNoSave);
		SetSaveButtonTitle();
	}	
}

void CHardWareConfig::OnWrongSave() 
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_WrongSave))->GetCheck())
	{
		CapsuleProc::SetSaveState(CapsuleProc::eErrSave);
		SetSaveButtonTitle();
	}
	else
	{
		CapsuleProc::SetSaveState(CapsuleProc::eNoSave);
		SetSaveButtonTitle();
	}
}

void CHardWareConfig::OnFrontLightEnable() 
{
	// TODO: Add your control notification handler code here

}

void CHardWareConfig::OnTransparent() 
{
	// TODO: Add your control notification handler code here
}
