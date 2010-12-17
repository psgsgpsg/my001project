// TBeacon.h: interface for the TBeacon class.
#ifndef TBEACON_H
#define TBEACON_H

#include "CtrlCardComm.h"
#include "TEvent.h"
#include "TCritSect.h"
#include "stdafx.h"
#include "CapsuleProc.h"

class TBeacon  
{
public:
	static TBeacon& TheBeacon();
	~TBeacon();
	bool	Trigger(EIndex camIndex);
	bool	Check();

	bool	RedShining	()	{	return m_redShining;	}
	bool	GreenShining()	{	return m_greenShining;	}
	bool	YellowShining()	{	return m_yellowShining;	}
	void	Alarm()			{	TurnRed(true);			}	
	bool	PushResult(EIndex camIndex, unsigned int result);
	void	ResetResult();

	
private:
	TBeacon();
	TBeacon(const TBeacon& src);
	TBeacon& operator= (const TBeacon& src);

	enum { CAMCOUNT = 2 };
	enum { ALARMLEN = 5	};
	


	enum
	{
		FrameInterCount	=	9,
		FramInterTime	= 200
	};

	void			TurnRed			(bool ifLight);
	void			TurnGreen		(bool ifLight);
	void			TurnYellow		(bool ifLight);
	bool			StaticResult	( EIndex camIndex);

	bool			m_greenShining;
	bool			m_yellowShining;
	bool			m_redShining;

	size_t			m_yellowCount;			//黄色灯延迟计数

	size_t			m_trigRecord;			//纪录触发
	size_t			m_lastTrigRecord;		//上一次纪录

	TCritSect		m_critSect;
	unsigned int	m_badArray[CAMCOUNT][CapsuleProc::BAD_ARRAY_LEN];

};

#endif

