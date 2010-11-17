// ProciliCard.h	: Interface of the ProciliCard class
//
// Copyright (c) 2007 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Module :ImageCard
// Create Date	: 2008.01.08
//
////////////////////////////////////////////////////////////////////
// Example begin
//
//
// ProciliCard card;
// card.Open();
// card.Live();
// { 线程自动调用 OnAcquistion(), 把图像处理的函数放于此成员函数中 }
// card.UnLive();
// card.Close();
// 
// Example end
/////////////////////////////////////////////////////////////////////
// A simple tool for bitflow Image card(rgb image card)

#ifndef PROCILICARD_H
#define PROCILICARD_H

#include "TImageCard.h"
#include "TAllocTmpl.h"
#include "TEvent.h"
#include "Procili/include/PvApi.h"

class ProciliCard : public TImageCard
{
public:
	ProciliCard();
	virtual ~ProciliCard();
	
public:	
	bool Open	(const char * camTag);					//open the card
	bool Open	();
	bool Close	();					//close the card
	
	bool StartLive	();				//start acquistion
	bool StopLive	();				//stop acquistion
	
	size_t	FrameInterval	( );					//get frame interval;
	bool	GrabTimeout		(	size_t ms);			//set grab timeout;
	bool	SetCamTag		(	const char* camTag);	//set Camera Tag, for example series number, name...

	bool	SetROI			(	const size_t	startX,
								const size_t	startY,
								const size_t	width,
								const size_t	height);

	bool SetExpTime (const size_t expTime);
	bool SetGain	(const size_t gain);

	bool SetWhiteBal(const size_t redVal,
					 const size_t greenVal,
					 const size_t blueVal);

	bool GetROI		(size_t &startX,
					 size_t &startY,
					 size_t &width,
					 size_t &height);

	bool GetWhiteBal(size_t &redVal,
					 size_t &greenVal,
					 size_t &blueVal);
	
	bool GetExpTime (size_t &expTime);
	bool GetGain	(size_t &gain);

	bool SetGrabMode(EnumGrabMode eMode);
	bool SaveSetting();
	
	TAlloc<PixelMem> GetPixelMem(); //Get Pixels memery
	void * GetPixelMemBase		() const;
	bool   RegisterProcFunc		(REGCallbackFunc* func, void *param); 
	
private:
	ProciliCard(const ProciliCard& src);
	ProciliCard& operator= (const ProciliCard& src);
	static void __stdcall FrameDoneCallBack(tPvFrame* pFrame);

private:
	enum			{ CAMBUFFCOUNT = 3 };
	tPvHandle		m_hCamera;
	char			m_grabMode[32];
	char			m_camTag[32];
	tPvFrame		m_frameInfo[CAMBUFFCOUNT];
	TAlloc<PixelMem>	m_pixleBuff[CAMBUFFCOUNT];
	size_t			m_pixelBuffIndex;

	size_t			m_grabTimeout;		//ms(单位：毫秒)
	TEvent			m_grabedEvent;

private:
	void*				m_pixelMemBase;
	REGCallbackFunc*	RegisteredFunc;
	void*				m_regFuncParam;
	double				m_stampInterval;
	unsigned long		m_stampFrequency;
	ULARGE_INTEGER		m_oldStamp;
};

#endif //PROCILICARD_H