// GenieCard.h	: Interface of the ProciliCard class
//
// Copyright (c) 2007 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Module :ImageCard
// Create Date	: 2008.01.24
//
////////////////////////////////////////////////////////////////////
// Example begin
//
//
// GenieCard card;
// card.Open();
// card.Live();
// { �߳��Զ����� OnAcquistion(), ��ͼ����ĺ������ڴ˳�Ա������ }
// card.UnLive();
// card.Close();
// 
// Example end
/////////////////////////////////////////////////////////////////////
// A simple tool for Genie Camera(rgb image card)

#ifndef GENIECARD_H
#define GENIECARD_H

#include "TImageCard.h"
#include "TAllocTmpl.h"

#include "Genie/Include/SapClassBasic.h"
#include "TEvent.h"

class GenieCard : public TImageCard
{
public:
	GenieCard();
	virtual ~GenieCard();
	
public:	
	bool Open	();					//open the card
	bool Open	(const char * camTag);					//open the card
	bool Close	();					//close the card
	
	bool StartLive	();				//start acquistion
	bool StopLive	();				//stop acquistion
	
	size_t	FrameInterval	( );			//get frame interval
	bool	GrabTimeout		(size_t ms);	//set grab timeout;
	
	bool SetROI		(size_t startX,
					 size_t startY,
					 size_t width,
					 size_t height);
	
	bool SetExpTime (size_t expTime);
	bool SetGain	(size_t gain);

	bool GetROI		(size_t &startX,
					 size_t &startY,
					 size_t &width,
					 size_t &height);
	
	bool GetExpTime (size_t &expTime);
	bool GetGain	(size_t &gain);

	bool SetGrabMode(EnumGrabMode eMode);
	bool SaveSetting();
	
	TAlloc<PixelMem> GetPixelMem(); //Get Pixels memery
	void * GetPixelMemBase		() const;
	bool   RegisterProcFunc		(REGCallbackFunc* func, void *param); 
	
private:
	GenieCard(const GenieCard& src);
	GenieCard& operator= (const GenieCard& src);
	bool CreateObjects();
	bool DeleteObjects();
	static void XferCallback(SapXferCallbackInfo *pInfo);	

private:
	enum			{ CAMBUFFCOUNT = 3 };
	SapAcqDevice	*m_acqDevice;
	SapBuffer		*m_buffers;
	SapTransfer		*m_xfer;
	
	unsigned int	m_grabTimeout;		//ms(��λ������)
	unsigned int	m_linePeriod;		//us(��λ��΢��)
	
	TEvent			m_grabedEvent;

private:
	void*				m_pixelMemBase;
	REGCallbackFunc*	RegisteredFunc;
	void*				m_regFuncParam;
	double				m_stampInterval;
};

#endif //GENIECARD_H