// TMMTimer.h	: Interface of the TMMTimer class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Last update Date	: 2006.12.28
//
// Example Begin
//
//  ����˵����
//	ʹ��TMMTimer ����ֱ��ʹ�ã������Ϊ����ʹ�á�
//  1. ֱ��ʹ��ʱ��ʹ��SetResolution(); �� SetMMTimerFunc();
//     ָ���������ںͻص���������.�ص�����������Ϊ: void Func(void* param);
//  example:
//	void exTimerFunc(void *param)
//  {
//		cout << "This is call callback function" << endl;
//  }
//  TMMTimer myTimer;
//	myTimer.SetResolution(50);
//  myTimer.SetMMTimerFunc(exTimerFunc, 0);
//	myTimer.Start(200,true);
//
//  2.������ʹ��ʱ�������麯��TimerProc().
//  example:
//  class myTimerClass : public TMMTimer
//  {	
//		public:
//			virtual	void TimerProc()
//			{
//			   cout << "This is call virtual function" << endl;
//			}
//  }
//
//	myThreadClass myTimer(50);
//	myThread.Start(200,false);
//  Sleep(3000);
//  myThread.Stop();
//
// Example End
//
// Simple MutiMedia timer control tool
// Last update Date	: 2006.12.28

#ifndef  TMMTIMER_H
#define  TMMTIMER_H

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include <mmsystem.h>	//winmm.lib

typedef void MMTimerFunc(void* param);

class TMMTimer
{
public:
	TMMTimer(UINT resolution = 10);	
	virtual ~TMMTimer();

	// set resolution and callback function
	bool	SetResolution (UINT resolution);
	bool	SetMMTimerFunc(MMTimerFunc* proc, void* param);


	// start the multimedia timer, 
	//when oneShot == true, it will execute one time only
	//when oneShot == false, it will one time one period
	bool	Start(UINT period,bool oneShot);

	// stop the multimedia timer
	bool	Stop();

public:
	virtual void TimerProc();

private:
	TMMTimer(const TMMTimer& src);
	TMMTimer& operator = (const TMMTimer& src);

	static void CALLBACK StaticTimerProc( UINT  uID,      
										  UINT  uMsg,     
										  DWORD dwUser,  
										  DWORD dw1,     
										  DWORD dw2);	

private:
	UINT	m_timerRes;
	UINT	m_timerID;
	MMTimerFunc	*MMTimerProc;
	void	*m_param;
};

#endif //TMMTIMER_H