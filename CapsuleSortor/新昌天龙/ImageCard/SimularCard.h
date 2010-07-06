//////////////////////////////////////////////////////////////////////////
//
// Module :ImageCard
// Create Date	: 2007.12.06
//
////////////////////////////////////////////////////////////////////
// Example begin
//
//
// SimularCard card;
// card.Open();
// card.Live();
// card.UnLive();
// card.Close();
// 
// Example end
/////////////////////////////////////////////////////////////////////
// A simple tool for simulated Image card(rgb image card)

#ifndef SIMULATEDCARD_H
#define SIMULATEDCARD_H

#include "TImageCard.h"
#include "TImgBuffer.h"
#include "TThread.h"
#include "TEvent.h"

class SimularCard : public TImageCard
{
public:
	SimularCard();
	virtual ~SimularCard();

public:	
	bool				Open			();				//open the card
	bool				Close			();				//close the card

	bool				StartLive		();				//start acquistion
	bool				StopLive		();				//stop acquistion

	size_t				FrameInterval	();
	TAlloc<PixelMem>	GetPixelMem		();				//Get Pixels memery
	bool				SoftTriggering	();				//softTrigger;
	void *				GetPixelMemBase	() const;
	bool				FrameRate		(	float			fps);	//set frame rate	
	bool				GrabTimeout		(	size_t			ms);	//set grab timeout;	
	bool				SetGrabMode		(	EnumGrabMode	eMode);	//set Grab Mode
						

	bool				RegisterProcFunc(	REGCallbackFunc	*func,
											void			*param); 

	// 设置仿真卡采集图像的来源
	bool				SetRawImage		(	const TImgBuffer	&rawImage);
private:
	SimularCard(const SimularCard& src);
	SimularCard& operator= (const SimularCard& src);
	static void			CreateImage(void *param);

private:
	REGCallbackFunc*	RegisteredFunc;
	void*				m_regFuncParam;
	
	TImgBuffer			m_imageSrc;
	TThread				m_thread;
	TEvent				m_grabEvent;
	TEvent				m_trigEvent;
	unsigned int		m_grabTimeout;
	unsigned int		m_frameInterval;
};

#endif //SIMULATEDCARD_H