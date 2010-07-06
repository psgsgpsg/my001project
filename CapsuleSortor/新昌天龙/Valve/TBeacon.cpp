
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
	if(m_greenShining != shining && !m_redShining)
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
	if(m_yellowShining != shining && !m_redShining)
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

bool	TBeacon::Trigger( const size_t curPeriod)
{
	TLocker lock(m_critSect);

	m_trigRecord++;
	if (curPeriod > FramInterTime)
	{
		if (m_yellowCount < FrameInterCount)
		{
			m_yellowCount++;
		}
		else
		{	
			TurnYellow(true);
		}	
	}
	else
	{	
		TurnGreen(true);
		m_yellowCount = 0;
	}
	return true;
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

