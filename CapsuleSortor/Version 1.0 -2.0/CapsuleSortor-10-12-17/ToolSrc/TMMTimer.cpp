// TMMTimer.cpp	: implement of the TMMTimer class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD



#include  "TMMTimer.h"

TMMTimer::TMMTimer(UINT resolution)
 : m_timerRes(0), m_timerID(0), MMTimerProc(0), m_param(0)
{
	SetResolution(resolution);
}

bool TMMTimer::SetResolution(UINT resolution)
{
	Stop();
	if (0 != m_timerRes)
	{
		timeEndPeriod(m_timerRes);
		m_timerRes = 0;
	}

	TIMECAPS	tc;
	if (TIMERR_NOERROR == timeGetDevCaps(&tc, sizeof(TIMECAPS)))
	{
		m_timerRes = min(max(tc.wPeriodMin,resolution),tc.wPeriodMax);
		timeBeginPeriod(m_timerRes);
	}
	else
	{
		m_timerRes = 0;
	}
	return (0 != m_timerRes);
}
bool TMMTimer::SetMMTimerFunc(MMTimerFunc* proc, void *param)
{
	Stop();

	MMTimerProc = proc;
	m_param		= param;

	return (0 != proc);
}

bool TMMTimer::Start(UINT period,bool oneShot)
{
	if(NULL == m_timerID)
	{
		m_timerID = timeSetEvent(	period,
									m_timerRes,
									TMMTimer::StaticTimerProc,
									(DWORD)this,
									oneShot ? TIME_ONESHOT : TIME_PERIODIC);
	}
	return m_timerID != NULL;
}


bool TMMTimer::Stop()
{
	if(NULL != m_timerID )
	{
		if(TIMERR_NOERROR == timeKillEvent(m_timerID) )
		{	m_timerID = NULL;	}
	}
	return NULL == m_timerID; 
}
TMMTimer::~TMMTimer()
{
	Stop();
	if (0 != m_timerRes)
	{
		timeEndPeriod(m_timerRes);
		m_timerRes = 0;
	}
}
void TMMTimer::TimerProc()
{ 
	if(MMTimerProc)
		MMTimerProc(m_param); 
}

void CALLBACK  TMMTimer::StaticTimerProc( UINT  uID,      
										  UINT  uMsg,     
										  DWORD dwUser,  
										  DWORD dw1,     
										  DWORD dw2)	
{
	TMMTimer * timer = (TMMTimer*)dwUser;
	timer->TimerProc();
}
