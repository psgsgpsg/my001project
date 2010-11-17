// A simple tool for Simular Image card. 

#include "SimularCard.h"

SimularCard::SimularCard()
: RegisteredFunc(0),
  m_regFuncParam(0),
  m_grabTimeout(10000), 
  m_frameInterval(5)
{
	m_thread.SetThreadFunc(SimularCard::CreateImage, this);	
}

SimularCard::~SimularCard()
{
	Close();
}

bool SimularCard::Open()
{
	if(m_opened)	return m_opened;

	m_roi.width			= 1024;
	m_roi.height		= 500;
	m_roi.bytesPerPixel	= 1;
	m_roi.bitsPerPixel	= m_roi.bytesPerPixel * 8;
	m_roi.bytesPerRow	= m_roi.bytesPerPixel * m_roi.width;

	m_pixelMem = TAlloc<PixelMem>(m_roi.bytesPerRow * m_roi.height, e16ByteAlign);

	m_opened = true;

	return m_opened;
}

bool SimularCard::Close()
{
	if(m_opened)
	{
		StopLive();
		m_opened = false;
	}
	return !m_opened;
}

bool SimularCard::FrameRate(float  fps)
{
	float realFPS	= (fps<0.1f)? 0.1f : (fps>100.0f)? 100.0f : fps;
	m_frameInterval = (unsigned int)(1000.0f/realFPS);
	return true;
}

size_t SimularCard::FrameInterval( )
{
	return m_frameInterval;
}

bool SimularCard::GrabTimeout(size_t ms)
{
	m_grabTimeout = ms;
	return true;
}

bool SimularCard::StartLive()
{
	if(!m_living)
	{
		if(!m_opened)
		{
			return m_living;
		}
		m_living = true;
		if(!m_thread.Start())
		{
			m_living = false;
		}
	}
	return m_living;
}

bool SimularCard::StopLive()
{
	if(m_living)
	{
		m_living = false;
		if(!m_thread.Wait(m_frameInterval*2))
		{
			m_thread.Stop();
		}
	}
	return !m_living;
}
void SimularCard::CreateImage(void *param)
{
	static size_t frameIndex = 0;
	SimularCard* pThis = (SimularCard*)(param);
	while(pThis->m_living)
	{
		if(pThis->GetGrabMode() == TImageCard::SOFTTRG)
		{
			pThis->m_trigEvent.Wait();
			pThis->m_trigEvent.ResetEvent();
		}
		
		pThis->m_imageSrc.GoNextFrame();
		
		//调用注册的处理函数或设置事件
		if(pThis->RegisteredFunc)
		{
			//pThis->RegisteredFunc(pThis->m_regFuncParam);
			pThis->m_imageSignal.PulseEvent();
		}
		else
		{
			memcpy(	(pThis->m_pixelMem).Base(),
					(pThis->m_imageSrc).CurrentFrameData(),
					(pThis->m_imageSrc).FrameBytes());

			pThis->m_grabEvent.SetEvent();
		}
		Sleep(pThis->m_frameInterval);
	}
}
TAlloc<TImageCard::PixelMem> SimularCard::GetPixelMem()
{
	bool ok = m_grabEvent.Wait();
	m_grabEvent.ResetEvent();
	if(ok)
	{
		return m_pixelMem;
	}
	else
	{
		return m_pixelMem.NullObject();
	}
}

bool SimularCard::SetRawImage(const TImgBuffer& rawImage)
{
	m_imageSrc	= rawImage;
	m_roi		= rawImage.Dimension();
	m_pixelMem	= TAlloc<PixelMem>(m_roi.bytesPerRow * m_roi.height, e16ByteAlign);
	return true;
}

bool SimularCard::SetGrabMode	(EnumGrabMode eMode)
{
	m_eGrabMode = eMode;
	return true;
}
bool SimularCard::SoftTriggering( )
{
	m_trigEvent.SetEvent();
	return true;
}

bool SimularCard::RegisterProcFunc(REGCallbackFunc* func, void *param)
{
	RegisteredFunc = func;
	m_regFuncParam = param;
	return true;
}

void* SimularCard::GetPixelMemBase() const
{
	return m_imageSrc.CurrentFrameData();
}


















