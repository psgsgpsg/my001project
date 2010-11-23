//////////////////////////////////////////////////////////////////////////
//
// Module :SVSGIGECARD.h
//
////////////////////////////////////////////////////////////////////
// Example begin
//
//
// SVSGigECard card;
// card.Open();
// card.StartLive();
// card.StopLive();
// card.Close();
// 
// Example end
/////////////////////////////////////////////////////////////////////


#ifndef SVSGIGECARD_H
#define SVSGIGECARD_H

#include "TImageCard.h"
#include "TAllocTmpl.h"
#include "Ttimediff.h"
#include "TEvent.h"
#include <string>
using namespace std;

#include "SVGigE.h"

class SVSGigECard : public TImageCard
{
public:
	SVSGigECard();
	virtual ~SVSGigECard();

public:	
	bool Open	();					
	bool Open(const char * camTag);
	bool Close	();					

	bool StartLive	();				
	bool StopLive	();				

	bool SetFrameRate(float  fps);	
	bool GetFrameRate(float  &fps);

	TAlloc<PixelMem> GetPixelMem(); 
	bool SetROI		(	const size_t startX,
						const size_t startY,
						const size_t width,
						const size_t height);
	bool SetExpTime	(	const size_t	expTime);
	bool SetGain	(	const size_t	gain);
	bool SetWhiteBal(	const size_t	redVal,
						const size_t	greenVal,
						const size_t	blueVal);
	bool GetExpTime	(	size_t	&expTime);
	bool GetWhiteBal(	size_t	&redVal,
						size_t	&greenVal,
						size_t	&blueVal);
	bool GetGain	(	size_t	&gain);
	bool GetROI		(	size_t	&startX,
						size_t	&startY,
						size_t	&width,
						size_t	&height);
	void* GetPixelMemBase	() const;
	bool  RegisterProcFunc	(REGCallbackFunc* func, void *param); 

	bool SaveSetting();

	size_t FrameInterval( );	

	
private:
	SVSGigECard(const SVSGigECard& src);
	SVSGigECard& operator= (const SVSGigECard& src);
	static SVGigE_RETURN __stdcall	ImageCallback(Image_handle Image, void* Context);
	void							SetCameraName(const char * camTag);
	bool							RefreshData();
	static CameraContainerClient_handle	m_hClient;

	TTimeDiff						m_timeCounter;
	
private:
	enum			{ CAMBUFFCOUNT = 3 };

	Camera_handle					m_hCamera;
	StreamingChannel_handle			m_hChannel;
	
	void*							m_regFuncParam;
	void*							m_pixelMemBase;
	string							m_cameraName;
	
	REGCallbackFunc*	RegisteredFunc;
	REGCallbackFunc*	m_regFuncStore;

	double				m_stampInterval;
	double				m_oldInterval;
	
};

#endif //SVSGIGECARD_H