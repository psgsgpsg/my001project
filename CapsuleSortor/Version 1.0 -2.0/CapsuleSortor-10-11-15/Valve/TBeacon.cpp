
#include "TBeacon.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TBeacon::TBeacon()
:	m_greenShining(false),
	m_yellowShining(false),
	m_redShining(false),
	m_yellowCount(0),			//黄色灯延迟计数
	m_trigRecord(0),			//纪录触发
	m_lastTrigRecord(0)		//上一次纪录
{	}

TBeacon::~TBeacon()
{
	CtrlCardComm::TheComm().LightCtrl(CLOSEALL, false);
}

TBeacon& TBeacon::TheBeacon()
{
	static TBeacon theBeacon;
	return theBeacon;
}

void	TBeacon::TurnRed			(bool shining)
{
	if(m_redShining != shining)
	{
		m_redShining = shining;
		if (m_redShining)	
		{
			m_greenShining	= false;
			m_yellowShining	= false;		
			CtrlCardComm::TheComm().LightCtrl	(CLOSEALL,	false);
			CtrlCardComm::TheComm().LightCtrl	(RED,		true);
		}
	}
}

void	TBeacon::TurnGreen		(bool shining)
{
	if(m_greenShining != shining )
	{
		m_greenShining = shining;
		if (m_greenShining)	
		{
			m_redShining	= false;
			m_yellowShining	= false;
			CtrlCardComm::TheComm().LightCtrl	(CLOSEALL, false);
			CtrlCardComm::TheComm().LightCtrl	(GREEN,		true);
		}
	}
}

void	TBeacon::TurnYellow		(bool shining)
{	
	if(m_yellowShining != shining )
	{
		m_yellowShining = shining;
		if (m_yellowShining )
		{
			m_greenShining	= false;
			m_redShining	= false;
			CtrlCardComm::TheComm().LightCtrl	(CLOSEALL, false);
			CtrlCardComm::TheComm().LightCtrl	(YELLOW,		true);
		}
	}
}

bool	TBeacon::Trigger( Valve::ETIndex camIndex)
{
	TLocker lock(m_critSect);
	
	bool ret =StaticResult(camIndex); 

	if (!ret)
	{
		Alarm();
	}

	return ret;
}

bool	TBeacon::Check()
{
	TLocker lock(m_critSect);
	if (m_trigRecord == m_lastTrigRecord)
	{
		TurnYellow(true);
	}								
	else
	{
		m_lastTrigRecord = m_trigRecord;
	}
	return true;
}

bool	TBeacon::PushResult(Valve::ETIndex camIndex, unsigned int result)
{
	for (int index = 0; index < CapsuleProc::BAD_ARRAY_LEN - 1; index++)
	{
		m_badArray[camIndex][index] = m_badArray[camIndex][index +1];
	}
	m_badArray[camIndex][CapsuleProc::BAD_ARRAY_LEN -1] = result;

	return true;
}

bool	TBeacon::StaticResult(Valve::ETIndex camIndex)
{
	unsigned int badCount[CapsuleProc::CAPSULENUM];
	memset(badCount, 0, sizeof(badCount));

	for (int posIndex = 0; posIndex < CapsuleProc::CAPSULENUM; posIndex ++)
	{
		for (int resIndex = 0; resIndex < CapsuleProc::BAD_ARRAY_LEN; resIndex ++)
		{
			if (m_badArray[camIndex][resIndex]&&(1<<posIndex))
			{
				badCount[posIndex]++;
			}			
		}
	}
	for (int index  = 0; index < CapsuleProc::CAPSULENUM; index++)
	{
		if(badCount[index] >= ALARMLEN)
		{
			return false;			
		}
	}
	return true;
}

void	TBeacon::ResetResult()
{
	memset(m_badArray[Valve::eFirst], 0, CapsuleProc::BAD_ARRAY_LEN * sizeof(unsigned int));
	memset(m_badArray[Valve::eSecond], 0, CapsuleProc::BAD_ARRAY_LEN * sizeof(unsigned int));
	TurnGreen(true);
}





