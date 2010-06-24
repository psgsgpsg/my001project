// ControlBoardSetting.cpp : implementation file
//

#include "stdafx.h"
#include "capsulesorter.h"
#include "ControlBoardSetting.h"
#include "capsulesorterdlg.h"
#include "KeyBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlBoardSetting dialog


CControlBoardSetting::CControlBoardSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CControlBoardSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CControlBoardSetting)
	m_camDelay = 0;
	m_camHold = 0;
	m_lightDelay = 0;
	m_lightHold = 0;
	m_shieldTime = 0;
	m_valveDelay = 0;
	m_valveHold = 0;
	m_camValveDistance1 = 0;
	m_camValveDistance2 = 0;
	m_axisRadius = 0;
	m_codeAdjust = 0;
	m_codePrecision = 0;
	m_pulseInterval = 0;
	m_capsuleInterval = 0;
	m_sensorIndex = _T("");
	m_camIndex = _T("");
	m_valveIndex = _T("");
	//}}AFX_DATA_INIT
}


void CControlBoardSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControlBoardSetting)
	DDX_Control(pDX, IDC_CAMINTER2, m_camInter_selection2);
	DDX_Control(pDX, IDC_CAMINTER1, m_camInter_selection1);
	DDX_Control(pDX, IDC_LAMP_COMB, m_lamp_selection);
	DDX_Control(pDX, IDC_VALVE_INDEX, m_valve_index);
	DDX_Control(pDX, IDC_WORKMODE, m_workMode);
	DDX_Control(pDX, IDC_WATCH_DOG_MODE, m_watchdog_mode);
	DDX_Text(pDX, IDC_CAM1_DELAY, m_camDelay);
	DDV_MinMaxLong(pDX, m_camDelay, 0, 2147483647);
	DDX_Text(pDX, IDC_CAM1_HOLD, m_camHold);
	DDV_MinMaxLong(pDX, m_camHold, 0, 2147483647);
	DDX_Text(pDX, IDC_LIGHT1_DELAY, m_lightDelay);
	DDV_MinMaxLong(pDX, m_lightDelay, 0, 2147483647);
	DDX_Text(pDX, IDC_LIGHT1_HOLD, m_lightHold);
	DDV_MinMaxLong(pDX, m_lightHold, 0, 2147483647);
	DDX_Text(pDX, IDC_SENSOR_SHIELD_EDT, m_shieldTime);
	DDV_MinMaxLong(pDX, m_shieldTime, 0, 2147483647);
	DDX_Text(pDX, IDC_VALVE_DELAY, m_valveDelay);
	DDX_Text(pDX, IDC_VALVE_HOLD, m_valveHold);
	DDX_Text(pDX, IDC_CAM_VALVE_DISTANCE1, m_camValveDistance1);
	DDV_MinMaxLong(pDX, m_camValveDistance1, 0, 2147483647);
	DDX_Text(pDX, IDC_CAM_VALVE_DISTANCE2, m_camValveDistance2);
	DDV_MinMaxLong(pDX, m_camValveDistance2, 0, 2147483647);
	DDX_Text(pDX, IDC_AXIS_RADIUS, m_axisRadius);
	DDV_MinMaxLong(pDX, m_axisRadius, 0, 2147483647);
	DDX_Text(pDX, IDC_CODE_ADJUST, m_codeAdjust);
	DDV_MinMaxLong(pDX, m_codeAdjust, 0, 2147483647);
	DDX_Text(pDX, IDC_CODE_PRECISION, m_codePrecision);
	DDV_MinMaxLong(pDX, m_codePrecision, 0, 2147483647);
	DDX_Text(pDX, IDC_PULSE_INTERVAL, m_pulseInterval);
	DDV_MinMaxLong(pDX, m_pulseInterval, 0, 2147483647);
	DDX_Text(pDX, IDC_CAPSUAL_INTERVAL, m_capsuleInterval);
	DDV_MinMaxLong(pDX, m_capsuleInterval, 0, 2147483647);
	DDX_CBString(pDX, IDC_SENSOR_COMB, m_sensorIndex);
	DDX_CBString(pDX, IDC_CAM_COMB, m_camIndex);
	DDX_CBString(pDX, IDC_VALVE_COMB, m_valveIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CControlBoardSetting, CDialog)
	//{{AFX_MSG_MAP(CControlBoardSetting)
	ON_BN_CLICKED(IDC_SET_TIMEORDER, OnSetTimeorder)
	ON_BN_CLICKED(IDC_SET_COMPLEX, OnSetComplex)
	ON_BN_CLICKED(IDC_TEST_VALVE, OnTestValve)
	ON_BN_CLICKED(IDC_TEST_LAMP, OnTestLamp)
	ON_BN_CLICKED(IDC_SET_SPACE, OnSetSpace)
	ON_BN_CLICKED(IDC_SET_OTHER, OnSetOther)
	ON_BN_CLICKED(IDC_SET_VALUE, OnSetValue)
	ON_BN_CLICKED(IDC_READ_CURRENT, OnReadCurrent)
	ON_BN_CLICKED(IDC_CtrlSoftKeyBD, OnCtrlSoftKeyBD)
	ON_CBN_SELCHANGE(IDC_CAM_COMB, OnSelchangeCamComb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlBoardSetting message handlers

void CControlBoardSetting::OnSetTimeorder() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_SET_TIMEORDER)->EnableWindow( FALSE);
	CtrlCardComm &theComm = CtrlCardComm::TheComm();
	theComm.PulseSet(atoi(m_camIndex), m_camDelay, m_camHold, m_lightDelay, m_lightHold);
	Sleep(100);
	theComm.SenserCfg(atoi(m_sensorIndex), m_shieldTime);

	size_t	curCamDelay = 0, curCamHold = 0, curLightDelay = 0, curLightHold = 0; 
	Sleep(200);
	
	if(theComm.GetCurState())
	{		
		curCamDelay	= theComm.GetCurState(theComm.CamDelayCode(atoi(m_camIndex))).data;
		curCamHold	= theComm.GetCurState(theComm.CamHoldCode(atoi(m_camIndex))).data;
		curLightDelay	= theComm.GetCurState(theComm.LampDelayCode(atoi(m_camIndex))).data;
		curLightHold 	= theComm.GetCurState(theComm.LampHoldCode(atoi(m_camIndex))).data;
		//curValveHold	= theComm.GetCurState(theComm.ValveCfgCode(atoi(m_valveIndex))).data;										
	}
	else
	{
		AfxMessageBox("读取数据失败！");
	}

	if (curCamDelay != m_camDelay || curCamHold != m_camHold || 
		curLightDelay != m_lightDelay || curLightHold != m_lightHold)
	{
		AfxMessageBox("设置数据失败！");
	}	
	else
	{
		AfxMessageBox("设置数据成功！");
	}
	GetDlgItem(IDC_SET_TIMEORDER)->EnableWindow(TRUE);
}

BOOL CControlBoardSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Title
	CString title;
	title.LoadString(IDS_CTRLCARDCFG_TITLE);
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

	//Control
	CtrlCardComm::TheComm().Open();
	((CComboBox*)GetDlgItem(IDC_SENSOR_COMB))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_CAM_COMB   ))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_VALVE_COMB ))->SetCurSel(0);
	
	m_camInter_selection1.SetCurSel	(0);
	m_camInter_selection2.SetCurSel	(1);
	m_valve_index.SetCurSel			(0);
	m_lamp_selection.SetCurSel		(0);
	m_watchdog_mode.SetCurSel		(0);
	m_workMode.SetCurSel			(0);
	//m_lamp_selection
	OnReadCurrent();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CControlBoardSetting::OnSetComplex() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_SET_COMPLEX)->EnableWindow(FALSE);
	CtrlCardComm &theComm = CtrlCardComm::TheComm();
	theComm.CountMode(m_workMode.GetCurSel() + 1);
	if (1 == m_watchdog_mode.GetCurSel())
	{
		theComm.WatchDog(TRUE);
	}
	else
	{
		theComm.WatchDog(FALSE);
	}

	theComm.ValveCfg(atoi(m_valveIndex), m_valveDelay, m_valveHold );
	ValveParam param	= Valve::TheValve().GetParam();
	Valve::TheValve().SetParam(param);
	size_t	curValveHold = 0, curCountMode = 0;
	
	Sleep(200);
	if(theComm.GetCurState())
	{		
		curValveHold	= theComm.GetCurState(theComm.ValveCfgCode(atoi(m_valveIndex))).data;										
		curCountMode	= theComm.GetCurState(theComm.CountModeCode()).high;
	}
	else
	{
		AfxMessageBox("不能读取当前值！");
	}

	if (	curValveHold		!= m_valveHold ||
			curCountMode	!= m_workMode.GetCurSel() + 1)
	{
		AfxMessageBox("设置失败!");
	}
	else
	{
		AfxMessageBox("设置成功!");
	}

//	CheckValue(curValveHold, m_valveHold);
	
	GetDlgItem(IDC_SET_COMPLEX)->EnableWindow(TRUE);
}

void CControlBoardSetting::OnTestValve() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CtrlCardComm::TheComm().ValveTest(m_valve_index.GetCurSel() + 1);	
}

void CControlBoardSetting::OnTestLamp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CtrlCardComm::TheComm().LightCtrl(m_lamp_selection.GetCurSel() +1, true);	
}

void CControlBoardSetting::OnSetSpace() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_SET_SPACE)->EnableWindow(FALSE);

	CtrlCardComm &theComm = CtrlCardComm::TheComm();
	theComm.CamValveDistance(m_camInter_selection1.GetCurSel() + 1, m_camValveDistance1);
	theComm.CamValveDistance(m_camInter_selection2.GetCurSel() + 1, m_camValveDistance2);
	
	size_t	curCamValveDistance1 = 0, curCamValveDistance2 = 0; 
	//Sleep(0);
	
	if(theComm.GetCurState())
	{		
		curCamValveDistance1	= theComm.GetCurState(theComm.CamValveDistCode(m_camInter_selection1.GetCurSel() +1)).data;
		curCamValveDistance2	= theComm.GetCurState(theComm.CamValveDistCode(m_camInter_selection2.GetCurSel() +1)).data;
	}
	else
	{
		AfxMessageBox("读取数据失败！");
	}
	
	if (curCamValveDistance1 != m_camValveDistance1 || curCamValveDistance2 != m_camValveDistance2 )
	{
		AfxMessageBox("设置数据失败！");
	}	
	else
	{
		AfxMessageBox("设置数据成功！");
	}

	GetDlgItem(IDC_SET_SPACE)->EnableWindow(TRUE);
}

void CControlBoardSetting::OnSetOther() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_SET_OTHER)->EnableWindow(FALSE);
	CtrlCardComm& theComm = CtrlCardComm::TheComm();
	theComm.CapsuleInterval	(m_capsuleInterval);
	Sleep(100);
	theComm.AxisRadius		(m_axisRadius);
	Sleep(100);
	theComm.RotCoderCfg		(m_codePrecision, m_codeAdjust);
	Sleep(100);
	theComm.PulseInterval	(m_pulseInterval);
	
	size_t	curCapsuleInterval = 0, curAxisRadius = 0, curCodePrecision = 0, curCodeAdjust = 0, curPulseInterval = 0; 

	Sleep(200);
	
	if(theComm.GetCurState())
	{		
		curAxisRadius		= theComm.GetCurState(theComm.AxisRadiusCode()).data;
		curCapsuleInterval	= theComm.GetCurState(theComm.CapsuleIntervalCode()).high;
		curCodePrecision	= theComm.GetCurState(theComm.RotCoderPrecisionCode()).data;
		curCodeAdjust		= theComm.GetCurState(theComm.RotCoderAdjustValCode()).data;
		curPulseInterval	= theComm.GetCurState(theComm.PulseIntervalCode()).high;
	}
	else
	{
		AfxMessageBox("读取数据失败！");
	}
	
	if (curAxisRadius != m_axisRadius || curCapsuleInterval != m_capsuleInterval ||
		curCodePrecision != m_codePrecision || curCodeAdjust != m_codeAdjust ||
		curPulseInterval != m_pulseInterval)
	{
			AfxMessageBox("设置数据失败！");
	}	
	else
	{
			AfxMessageBox("设置数据成功！");
	}

	GetDlgItem(IDC_SET_OTHER)->EnableWindow(TRUE);
	
}

void CControlBoardSetting::OnSetValue() 
{
	// TODO: Add your control notification handler code here
	
}

void CControlBoardSetting::OnReadCurrent() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CtrlCardComm& theComm = CtrlCardComm::TheComm();
	
	if(theComm.GetCurState())
	{
		unsigned char   camIndex	= static_cast<unsigned char>(atoi(m_camIndex));
		m_camDelay	= theComm.GetCurState(theComm.CamDelayCode(camIndex)).data;
		m_camHold	= theComm.GetCurState(theComm.CamHoldCode(camIndex)).data;
		m_lightDelay= theComm.GetCurState(theComm.LampDelayCode(camIndex)).data;
		m_lightHold	= theComm.GetCurState(theComm.LampHoldCode(camIndex)).data;
		unsigned char   sensorIndex = static_cast<unsigned char>(atoi(m_sensorIndex));
		m_shieldTime= theComm.GetCurState(theComm.SenserCfgCode(sensorIndex)).data;

		m_valveHold	= theComm.GetCurState(theComm.ValveCfgCode(atoi(m_valveIndex))).data;
		unsigned short  countMode	= theComm.GetCurState(theComm.CountModeCode()).high;
		m_workMode.SetCurSel(countMode - 1);
		unsigned short  watchDog	= theComm.GetCurState(theComm.WatchDogCode()).high;
		m_watchdog_mode.SetCurSel(watchDog);
		m_camValveDistance1			= theComm.GetCurState(theComm.CamValveDistCode(m_camInter_selection1.GetCurSel() +1)).data;
		m_camValveDistance2			= theComm.GetCurState(theComm.CamValveDistCode(m_camInter_selection2.GetCurSel() +1)).data;
		m_capsuleInterval			= theComm.GetCurState(theComm.CapsuleIntervalCode()).high;
		m_pulseInterval				= theComm.GetCurState(theComm.PulseIntervalCode()).high;
		m_codePrecision				= theComm.GetCurState(theComm.RotCoderPrecisionCode()).data;
		m_codeAdjust				= theComm.GetCurState(theComm.RotCoderAdjustValCode()).high;
		m_axisRadius				= theComm.GetCurState(theComm.AxisRadiusCode()).data;									
	}
	UpdateData(FALSE);
}

void CControlBoardSetting::OnCtrlSoftKeyBD() 
{
	// TODO: Add your control notification handler code here
	CKeyBoardDlg::DisplayWindow();	
}

void CControlBoardSetting::CheckValue(size_t curValue, size_t stdValue)
{
	if (curValue != stdValue)
	{
		AfxMessageBox("设置失败!");
	}
	else
	{
		AfxMessageBox("设置成功!");
	}
}

void CControlBoardSetting::OnSelchangeCamComb() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	
	CtrlCardComm& theComm = CtrlCardComm::TheComm();
	
	if(theComm.GetCurState())
	{
		unsigned char   camIndex	= static_cast<unsigned char>(((CComboBox*)GetDlgItem(IDC_CAM_COMB   ))->GetCurSel() + 1);
		m_camIndex.Format("%d", camIndex);
		m_camDelay	= theComm.GetCurState(theComm.CamDelayCode(camIndex)).data;
		m_camHold	= theComm.GetCurState(theComm.CamHoldCode(camIndex)).data;
		m_lightDelay= theComm.GetCurState(theComm.LampDelayCode(camIndex)).data;
		m_lightHold	= theComm.GetCurState(theComm.LampHoldCode(camIndex)).data;
		unsigned char   sensorIndex = static_cast<unsigned char>(atoi(m_sensorIndex));
		m_shieldTime= theComm.GetCurState(theComm.SenserCfgCode(sensorIndex)).data;
	}		
	UpdateData(FALSE);
}
