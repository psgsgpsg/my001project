#include "stdafx.h"
#include "ConfigInfo.h"
#include "TIniFile.h"

ConfigInfo::ConfigInfo()
{
	m_workMode	= 0;
	m_solution_X = 0;
	m_solution_Y = 0;
}

ConfigInfo::~ConfigInfo()
{	}

/*******************************************************************************
*	函数名称：	TheConfigInfo
*	
*	参数	：	
*			  
*	输出	：	得到配置信息的引用
*
*	功能	：	得到配置信息
*
*
********************************************************************************/

ConfigInfo& ConfigInfo::TheConfigInfo()
{
	static ConfigInfo theCfgInfo;
	return theCfgInfo;
}
/*******************************************************************************
*	函数名称：	LoadIniFile
*	
*	参数	：	
*			  
*	输出	：	得到配置信息的引用
*
*	功能	：	得到配置信息
*
*
********************************************************************************/

bool ConfigInfo::LoadIniFile(const TString&		fileName,
									TIniFile::EnumPath	ePath)
{
	if(!TIniFile::SetFileName(fileName, ePath))	
	{
		AfxMessageBox( "载入配置文件失败！");
		return false;
	}
	
	m_simlation		= TIniFile::GetInt("SysConfig",		"Simulator",	1);
	m_workMode		= TIniFile::GetInt("SysConfig",		"WorkMode",		0);
	m_solution_X	= TIniFile::GetInt("SysConfig",		"Solution_X",	0);
	m_solution_Y	= TIniFile::GetInt("SysConfig",		"Solution_Y",	0);
	

	m_firstCapsuleParam.capsuleDim.tolerance	= TIniFile::GetInt("FirstCapsuleParam",		"Tolerance",		10	);
	m_firstCapsuleParam.capsuleDim.width		= TIniFile::GetInt("FirstCapsuleParam",		"Width",			138	);
	m_firstCapsuleParam.capsuleDim.height 		= TIniFile::GetInt("FirstCapsuleParam",		"Height",			380	);
	m_firstCapsuleParam.capColor.L				= TIniFile::GetInt("FirstCapsuleParam",		"Cap_L",			0	);
	m_firstCapsuleParam.capColor.a				= TIniFile::GetInt("FirstCapsuleParam",		"Cap_a"	,			0	);
	m_firstCapsuleParam.capColor.b	 			= TIniFile::GetInt("FirstCapsuleParam",		"Cap_b"	,			0	);
	m_firstCapsuleParam.capLength				= TIniFile::GetInt("FirstCapsuleParam",		"Cap_Length",		0	);
	m_firstCapsuleParam.bodyColor.L				= TIniFile::GetInt("FirstCapsuleParam",		"Body_L",			0	);
	m_firstCapsuleParam.bodyColor.a				= TIniFile::GetInt("FirstCapsuleParam",		"Body_a",			0	);
	m_firstCapsuleParam.bodyColor.b	 			= TIniFile::GetInt("FirstCapsuleParam",		"Body_b",			0	);
	m_firstCapsuleParam.bodyLength				= TIniFile::GetInt("FirstCapsuleParam",		"Body_Length",		0	);
	m_firstCapsuleParam.partTolerance			= TIniFile::GetInt("FirstCapsuleParam",		"PartTolerance",	0	);
	
	m_secondCapsuleParam.capsuleDim.tolerance	= TIniFile::GetInt("SecondCapsuleParam",	"Tolerance",		10	);
	m_secondCapsuleParam.capsuleDim.width		= TIniFile::GetInt("SecondCapsuleParam",	"Width",			138	);
	m_secondCapsuleParam.capsuleDim.height 		= TIniFile::GetInt("SecondCapsuleParam",	"Height",			380	);
	m_secondCapsuleParam.capColor.L				= TIniFile::GetInt("SecondCapsuleParam",	"Cap_L",			0	);
	m_secondCapsuleParam.capColor.a				= TIniFile::GetInt("SecondCapsuleParam",	"Cap_a"	,			0	);
	m_secondCapsuleParam.capColor.b	 			= TIniFile::GetInt("SecondCapsuleParam",	"Cap_b"	,			0	);
	m_secondCapsuleParam.capLength				= TIniFile::GetInt("SecondCapsuleParam",	"Cap_Length",		0	);
	m_secondCapsuleParam.bodyColor.L			= TIniFile::GetInt("SecondCapsuleParam",	"Body_L",			0	);
	m_secondCapsuleParam.bodyColor.a			= TIniFile::GetInt("SecondCapsuleParam",	"Body_a",			0	);
	m_secondCapsuleParam.bodyColor.b	 		= TIniFile::GetInt("SecondCapsuleParam",	"Body_b",			0	);
	m_secondCapsuleParam.bodyLength				= TIniFile::GetInt("SecondCapsuleParam",	"Body_Length",		0	);
	m_secondCapsuleParam.partTolerance			= TIniFile::GetInt("SecondCapsuleParam",	"PartTolerance",	0	);	

	m_secondParam.maxValue						= TIniFile::GetInt("MonoSortor",	"MaxValue",			230	);
	m_secondParam.shrinkX						= TIniFile::GetInt("MonoSortor",	"ShrinkX",			10	);
	m_secondParam.shrinkY						= TIniFile::GetInt("MonoSortor",	"ShrinkY",			5	);
	
	m_secondParam.edgeThres						= TIniFile::GetInt("MonoSortor",	"EdgeThres",		3	);
	m_secondParam.edgeDensity					= TIniFile::GetInt("MonoSortor",	"EdgeDensity",		500	);
	m_secondParam.dynThres						= TIniFile::GetInt("MonoSortor",	"DynThres",			6	);
	m_secondParam.dynWndSize					= TIniFile::GetInt("MonoSortor",	"dynWndSize",		20	);
	m_secondParam.blobSize						= TIniFile::GetInt("MonoSortor",	"BlobSize",			15	);
	m_secondParam.transparent					= TIniFile::GetInt("MonoSortor",	"Transparent",		0	);
	
	m_firstParam.maxValue						= TIniFile::GetInt("ColorSortor",	"MaxValue",			230	);
	m_firstParam.shrinkX						= TIniFile::GetInt("ColorSortor",	"ShrinkX",			15	);
	m_firstParam.shrinkY						= TIniFile::GetInt("ColorSortor",	"ShrinkY",			8	);
	
	m_firstParam.edgeThres						= TIniFile::GetInt("ColorSortor",	"EdgeThres",		2	);
	m_firstParam.edgeDensity					= TIniFile::GetInt("ColorSortor",	"EdgeDensity",		300	);
	m_firstParam.dynThres						= TIniFile::GetInt("ColorSortor",	"DynThres",			6	);
	m_firstParam.dynWndSize						= TIniFile::GetInt("ColorSortor",	"DynWndSize",		30	);
	m_firstParam.blobSize						= TIniFile::GetInt("ColorSortor",	"BlobSize",			15	);
	m_firstParam.transparent					= TIniFile::GetInt("ColorSortor",	"Transparent",		0	);

	m_secondCamParam.roiStartX					= TIniFile::GetInt("SecondCam",		"ROIStartX",		0	);
	m_secondCamParam.roiStartY					= TIniFile::GetInt("SecondCam",		"ROIStartY",		220	);
	m_secondCamParam.roiWidth					= TIniFile::GetInt("SecondCam",		"ROIWidth",			1024);
	m_secondCamParam.roiHeight					= TIniFile::GetInt("SecondCam",		"ROIHeight"	,		480	);
	m_secondCamParam.gain						= TIniFile::GetInt("SecondCam",		"Gain"		,		0	);
	m_secondCamParam.expTime					= TIniFile::GetInt("SecondCam",		"ExpTime"	,		500	);
	m_secondCamParam.balanceRed					= TIniFile::GetInt("SecondCam",		"BalanceRed",		100	);
	m_secondCamParam.balanceGreen				= TIniFile::GetInt("SecondCam",		"BalanceGreen",		100	);
	m_secondCamParam.balanceBlue				= TIniFile::GetInt("SecondCam",		"BalanceBlue",		100	);
	m_secondCamParam.camTag						= TIniFile::GetStr("SecondCam",		"CamTag");
	

	m_firstCamParam.roiStartX					= TIniFile::GetInt("FirstCam",	"ROIStartX"	,			0	);
	m_firstCamParam.roiStartY					= TIniFile::GetInt("FirstCam",	"ROIStartY"	,			220	);
	m_firstCamParam.roiWidth					= TIniFile::GetInt("FirstCam",	"ROIWidth"	,			1024);
	m_firstCamParam.roiHeight					= TIniFile::GetInt("FirstCam",	"ROIHeight"	,			480	);
	m_firstCamParam.gain						= TIniFile::GetInt("FirstCam",	"Gain"		,			0	);
	m_firstCamParam.expTime						= TIniFile::GetInt("FirstCam",	"ExpTime"	,			1000);
	m_firstCamParam.balanceRed					= TIniFile::GetInt("FirstCam",	"BalanceRed",			174	);
	m_firstCamParam.balanceGreen				= TIniFile::GetInt("FirstCam",	"BalanceGreen",			100	);
	m_firstCamParam.balanceBlue					= TIniFile::GetInt("FirstCam",	"BalanceBlue",			257	);
	m_firstCamParam.camTag						= TIniFile::GetStr("FirstCam",	"CamTag"					);
	
	m_valveParam.IOBoard						= TIniFile::GetInt("ValveCtrl",	"IOBoard"	,			0	);
	m_valveParam.ResultInterval					= TIniFile::GetInt("ValveCtrl",	"ResultInterval",		5	);
	
	m_comCtrl.baudRate							= TIniFile::GetInt("ComCtrl",	"BaudRate"	,			57600);
	m_comCtrl.portIndex							= TIniFile::GetInt("ComCtrl",	"PortIndex"	,			1	);

	m_colorRemain.radius						= TIniFile::GetInt("Remain",	"ColorRadius",			58);
	m_colorRemain.minBoxWidth					= TIniFile::GetInt("Remain",	"ColorMinBoxWidth",		15);
	m_colorRemain.binaryLowThres				= TIniFile::GetInt("Remain",	"ColorBinaryLowThres",	50);
	m_colorRemain.binaryUpThres					= TIniFile::GetInt("Remain",	"ColorBinaryUpThres",	255);

	m_monoRemain.radius							= TIniFile::GetInt("Remain",	"MonoRadius",			58);
	m_monoRemain.minBoxWidth					= TIniFile::GetInt("Remain",	"MonoMinboxWidth",		15);
	m_monoRemain.binaryLowThres					= TIniFile::GetInt("Remain",	"MonoBinaryLowThres",	50);
	m_monoRemain.binaryUpThres					= TIniFile::GetInt("Remain",	"MonoBinaryUpThres",	255);

	m_radiumRange.minRadium						= TIniFile::GetInt("RadiumRange","minRadium",			50);
	m_radiumRange.maxRadium						= TIniFile::GetInt("RadiumRange","maxRadium",			60);

	m_minboxWidthRange.minWidth					= TIniFile::GetInt("MinboxWidthRange", "minWidth",		0);
	m_minboxWidthRange.maxWidth					= TIniFile::GetInt("MinboxWidthRange",	"maxWidth",		20);
	return true;
}

bool ConfigInfo::SaveIniFile(const TString&	fileName,
							 TIniFile::EnumPath	ePath)
{
	TIniFile::SetFileName(fileName, ePath);

	TIniFile::SetInt("SysConfig",		"Simulator",		m_simlation							);		
	TIniFile::SetInt("FirstCapsuleParam",	"Tolerance",		m_firstCapsuleParam.capsuleDim.tolerance	);
	TIniFile::SetInt("FirstCapsuleParam",	"Width",			m_firstCapsuleParam.capsuleDim.width		);
	TIniFile::SetInt("FirstCapsuleParam",	"Height",			m_firstCapsuleParam.capsuleDim.height	);
	TIniFile::SetInt("FirstCapsuleParam",	"Cap_L",			m_firstCapsuleParam.capColor.L			);
	TIniFile::SetInt("FirstCapsuleParam",	"Cap_a",			m_firstCapsuleParam.capColor.a			);
	TIniFile::SetInt("FirstCapsuleParam",	"Cap_b",			m_firstCapsuleParam.capColor.b			);
	TIniFile::SetInt("FirstCapsuleParam",	"Body_L",			m_firstCapsuleParam.bodyColor.L			);
	TIniFile::SetInt("FirstCapsuleParam",	"Body_a",			m_firstCapsuleParam.bodyColor.a			);
	TIniFile::SetInt("FirstCapsuleParam",	"Body_b",			m_firstCapsuleParam.bodyColor.b			);
	TIniFile::SetInt("FirstCapsuleParam",	"Body_Length",		m_firstCapsuleParam.bodyLength			);
	TIniFile::SetInt("FirstCapsuleParam",	"Cap_Length",		m_firstCapsuleParam.capLength			);
	TIniFile::SetInt("FirstCapsuleParam",	"PartTolerance",	m_firstCapsuleParam.partTolerance		);

	TIniFile::SetInt("SecondCapsuleParam",	"Tolerance",		m_secondCapsuleParam.capsuleDim.tolerance	);
	TIniFile::SetInt("SecondCapsuleParam",	"Width",			m_secondCapsuleParam.capsuleDim.width		);
	TIniFile::SetInt("SecondCapsuleParam",	"Height",			m_secondCapsuleParam.capsuleDim.height	);
	TIniFile::SetInt("SecondCapsuleParam",	"Cap_L",			m_secondCapsuleParam.capColor.L			);
	TIniFile::SetInt("SecondCapsuleParam",	"Cap_a",			m_secondCapsuleParam.capColor.a			);
	TIniFile::SetInt("SecondCapsuleParam",	"Cap_b",			m_secondCapsuleParam.capColor.b			);
	TIniFile::SetInt("SecondCapsuleParam",	"Body_L",			m_secondCapsuleParam.bodyColor.L			);
	TIniFile::SetInt("SecondCapsuleParam",	"Body_a",			m_secondCapsuleParam.bodyColor.a			);
	TIniFile::SetInt("SecondCapsuleParam",	"Body_b",			m_secondCapsuleParam.bodyColor.b			);
	TIniFile::SetInt("SecondCapsuleParam",	"Body_Length",		m_secondCapsuleParam.bodyLength			);
	TIniFile::SetInt("SecondCapsuleParam",	"Cap_Length",		m_secondCapsuleParam.capLength			);
	TIniFile::SetInt("SecondCapsuleParam",	"PartTolerance",	m_secondCapsuleParam.partTolerance		);
	
	TIniFile::SetInt("MonoSortor",		"MaxValue",			m_secondParam.maxValue				);
	TIniFile::SetInt("MonoSortor",		"ShrinkX",			m_secondParam.shrinkX				);
	TIniFile::SetInt("MonoSortor",		"ShrinkY",			m_secondParam.shrinkY				);
	
	TIniFile::SetInt("MonoSortor",		"EdgeThres",		m_secondParam.edgeThres				);
	TIniFile::SetInt("MonoSortor",		"EdgeDensity",		m_secondParam.edgeDensity			);
	TIniFile::SetInt("MonoSortor",		"DynThres",			m_secondParam.dynThres				);
	TIniFile::SetInt("MonoSortor",		"DynWndSize",		m_secondParam.dynWndSize			);
	TIniFile::SetInt("MonoSortor",		"BlobSize",			m_secondParam.blobSize				);
	TIniFile::SetInt("MonoSortor",		"Transparent",		m_secondParam.transparent			);
	
	TIniFile::SetInt("ColorSortor",		"MaxValue",			m_firstParam.maxValue			);
	TIniFile::SetInt("ColorSortor",		"ShrinkX",			m_firstParam.shrinkX			);
	TIniFile::SetInt("ColorSortor",		"ShrinkY",			m_firstParam.shrinkY			);
	
	TIniFile::SetInt("ColorSortor",		"EdgeThres",		m_firstParam.edgeThres			);
	TIniFile::SetInt("ColorSortor",		"EdgeDensity",		m_firstParam.edgeDensity		);
	TIniFile::SetInt("ColorSortor",		"DynThres",			m_firstParam.dynThres			);
	TIniFile::SetInt("ColorSortor",		"DynWndSize",		m_firstParam.dynWndSize			);
	TIniFile::SetInt("ColorSortor",		"BlobSize",			m_firstParam.blobSize			);
	TIniFile::SetInt("ColorSortor",		"Transparent",		m_firstParam.transparent		);

	TIniFile::SetInt("SecondCam",		"ROIStartX",		m_secondCamParam.roiStartX		);
	TIniFile::SetInt("SecondCam",		"ROIStartY",		m_secondCamParam.roiStartY		);
	TIniFile::SetInt("SecondCam",		"ROIWidth",			m_secondCamParam.roiWidth		);
	TIniFile::SetInt("SecondCam",		"ROIHeight",		m_secondCamParam.roiHeight		);
	TIniFile::SetInt("SecondCam",		"Gain",				m_secondCamParam.gain			);
	TIniFile::SetInt("SecondCam",		"ExpTime",			m_secondCamParam.expTime		);
	TIniFile::SetInt("SecondCam",		"BalanceRed",		m_secondCamParam.balanceRed		);
	TIniFile::SetInt("SecondCam",		"BalanceGreen",		m_secondCamParam.balanceGreen	);
	TIniFile::SetInt("SecondCam",		"BalanceBlue",		m_secondCamParam.balanceBlue	);
	TIniFile::SetStr("SecondCam",		"CamTag",			m_secondCamParam.camTag			);
	
	TIniFile::SetInt("FirstCam",		"ROIStartX",		m_firstCamParam.roiStartX		);
	TIniFile::SetInt("FirstCam",		"ROIStartY",		m_firstCamParam.roiStartY		);
	TIniFile::SetInt("FirstCam",		"ROIWidth",			m_firstCamParam.roiWidth		);
	TIniFile::SetInt("FirstCam",		"ROIHeight",		m_firstCamParam.roiHeight		);
	TIniFile::SetInt("FirstCam",		"Gain",				m_firstCamParam.gain			);
	TIniFile::SetInt("FirstCam",		"ExpTime",			m_firstCamParam.expTime			);
	TIniFile::SetInt("FirstCam",		"BalanceRed",		m_firstCamParam.balanceRed		);
	TIniFile::SetInt("FirstCam",		"BalanceGreen",		m_firstCamParam.balanceGreen	);
	TIniFile::SetInt("FirstCam",		"BalanceBlue",		m_firstCamParam.balanceBlue		);
	TIniFile::SetStr("FirstCam",		"CamTag",			m_firstCamParam.camTag			);

	TIniFile::SetInt("ValveCtrl",		"IOBoard"	,	m_valveParam.IOBoard			);
	TIniFile::SetInt("ValveCtrl",		"ResultInterval",	m_valveParam.ResultInterval		);

	TIniFile::SetInt("ComCtrl",			"BaudRate",			m_comCtrl.baudRate				);
	TIniFile::SetInt("ComCtrl",			"PortIndex",		m_comCtrl.portIndex				);

	TIniFile::SetInt("RadiumRange",		"minRadium",			m_radiumRange.minRadium			);
	TIniFile::SetInt("RadiumRange",		"maxRadium",			m_radiumRange.maxRadium			);
	
	TIniFile::SetInt("Remain",			"ColorRadius",			m_colorRemain.radius		);
	TIniFile::SetInt("Remain",			"ColorMinBoxWidth",		m_colorRemain.minBoxWidth	);
	TIniFile::SetInt("Remain",			"ColorBinaryLowThres",	m_colorRemain.binaryLowThres);
	TIniFile::SetInt("Remain",			"ColorBinaryUpThres",	m_colorRemain.binaryUpThres);

	TIniFile::SetInt("Remain",			"MonoRadius",			m_monoRemain.radius		);
	TIniFile::SetInt("Remain",			"MonoMinBoxWidth",		m_monoRemain.minBoxWidth	);
	TIniFile::SetInt("Remain",			"MonoBinaryLowThres",	m_monoRemain.binaryLowThres);
	TIniFile::SetInt("Remain",			"MonoBinaryUpThres",	m_monoRemain.binaryUpThres);
	
	TIniFile::SetInt("MinboxWidthRange",	"minWidth",			m_minboxWidthRange.minWidth);
	TIniFile::SetInt("MinboxWidthRange",	"maxWidth",			m_minboxWidthRange.maxWidth);
	
	return true;
}

//const CapsuleParam& ConfigInfo::CapsuleInfo()	const
//{	return m_capsuleParam;	}

const SortorParam& ConfigInfo::ColorInfo		()	const
{	return m_firstParam;	}

const SortorParam& ConfigInfo::MonoInfo			()	const
{	return m_secondParam;		}

const CameraParam& ConfigInfo::SecondCamInfo	()	const
{	return m_secondCamParam;}

const CameraParam& ConfigInfo::FirstCamInfo		()const
{	return m_firstCamParam;	}

const ValveParam& ConfigInfo::ValveInfo			()	const
{	return m_valveParam;	}
const ComCtrlParam& ConfigInfo::ComCtrlInfo		()  const
{	return	m_comCtrl;	}
const RemainParam&	ConfigInfo::RemainInfo		(	PROTYPE type)	const
{
	switch(type)
	{
		case FIRST:
			return m_colorRemain;
		case SECOND:
			return m_monoRemain;
		default:	
			return m_monoRemain;		
	}
}

bool ConfigInfo::Simulation			()	const
{	return (m_simlation  != 0) ? true : false;	}

void ConfigInfo::ColorInfo			(	const SortorParam&		param)
{	m_firstParam = param;			}

void ConfigInfo::MonoInfo			(	const SortorParam&		param)
{	m_secondParam = param;			}

void ConfigInfo::SecondCamInfo		(	const CameraParam&		param)
{	m_secondCamParam = param;		}

void ConfigInfo::FirstCamInfo		(	const CameraParam&		param)
{	m_firstCamParam = param;		}

void ConfigInfo::ValveInfo			(	const ValveParam&		param)
{	m_valveParam = param;			}
void ConfigInfo::ComCtrlInfo		(	const ComCtrlParam&		param)
{	m_comCtrl	=	param;			}
const CapsuleParam& ConfigInfo::FirstCapsuleInfo()	const
{	return m_firstCapsuleParam;		}

const CapsuleParam&	ConfigInfo::SecondCapsuleInfo() const
{	return m_secondCapsuleParam;	}

void ConfigInfo::FirstCapsuleInfo	(	const CapsuleParam&		param)
{	m_firstCapsuleParam = param;	}
 
void ConfigInfo::SecondCapsuleInfo	(	const CapsuleParam&		param)
{	m_secondCapsuleParam = param;	}

const RadiumRange&	ConfigInfo::RadiumRangeInfo	()	const
{
	return m_radiumRange;
}

void ConfigInfo::RemainInfo			(	PROTYPE type, const RemainParam&		param)
{
	switch(type)
	{
	case FIRST:
		m_colorRemain = param;
		break;
	case SECOND:
		m_monoRemain = param;
		break;
	}
}


const MinboxWidthRange&	ConfigInfo::MinboxWidthInfo	() const
{
	return m_minboxWidthRange;
}

int		ConfigInfo::WorkMode		()	const
{
	return m_workMode;
}
const int	ConfigInfo::Solution_X		()	const
{
	return  m_solution_X;
}
const int	ConfigInfo::Solution_Y		()	const
{
	return m_solution_Y;
}