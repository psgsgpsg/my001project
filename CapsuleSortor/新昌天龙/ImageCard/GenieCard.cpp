// GenieCard.cpp	: Interface of the GenieCard class
//
// Copyright (c) 2006 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Module :ImageCard
// Create Date	: 2008.01.24
//
// A simple tool for Genie Camera. 

#include "GenieCard.h"
#include "TTimeDiff.h"

GenieCard::GenieCard()
: m_grabTimeout(1000),
  m_linePeriod(125),
  m_acqDevice(0),
  m_buffers(0),
  m_xfer(0),
  m_pixelMemBase(0),
  RegisteredFunc(0),
  m_regFuncParam(0),
  m_stampInterval(0)
{
}

GenieCard::~GenieCard()
{
	Close();
}

bool GenieCard::CreateObjects(const char * camTag)
{
	//SapLocation::ServerSystem
	if(camTag)
	{
		m_acqDevice	= new SapAcqDevice	(camTag, 0);
	}
	else
	{
		m_acqDevice = new SapAcqDevice("Genie_M1024_1", 0);
	}
	m_buffers	= new SapBufferWithTrash(CAMBUFFCOUNT, m_acqDevice);
	m_xfer		= new SapAcqDeviceToBuf	(m_acqDevice, m_buffers, XferCallback, this);

	int cameraCount = m_acqDevice->GetServerCount();
	
	if ( 0 == cameraCount)
	{
		return false;
	}
	// Create acquisition object

	
	if (m_acqDevice && !*m_acqDevice)
	{
		BOOL ok = m_acqDevice->SetConfigFile(camTag);
		if(FALSE != ok && !m_acqDevice->Create())
		{
			DeleteObjects();
			return false;
		}
	}
	
	// Create buffer object
	if (m_buffers && !*m_buffers)
	{
		if( !m_buffers->Create())
		{
			DeleteObjects();
			return false;
		}
		// Clear all buffers
		m_buffers->Clear();
	}
	
	// Create transfer object
	if (m_xfer && !*m_xfer && !m_xfer->Create())
	{
		DeleteObjects();
		return false;
	}
	return true;
}
bool GenieCard::DeleteObjects()
{
	// Destroy transfer object
	if (m_xfer && *m_xfer) m_xfer->Destroy();
	
	// Destroy buffer object
	if (m_buffers && *m_buffers) m_buffers->Destroy();
	
	// Destroy acquisition object
	if (m_acqDevice && *m_acqDevice) m_acqDevice->Destroy();


	// Delete all objects
	if (m_xfer)
	{
		delete m_xfer; 
		m_xfer = 0;
	}

	if (m_buffers)
	{
		delete m_buffers; 
		m_buffers = 0;
	}

	if (m_acqDevice)
	{
		delete m_acqDevice;
		m_acqDevice = 0;
	}
	return true;
}

bool GenieCard::Open(const char * camTag)
{
	if(m_opened)	return m_opened;

	m_opened = CreateObjects(camTag);
	
	if(m_opened)
	{
		m_roi.width = m_buffers->GetWidth();
		m_roi.height = m_buffers->GetHeight();
		m_roi.bitsPerPixel = m_buffers->GetPixelDepth();
		m_roi.bytesPerPixel = m_buffers->GetBytesPerPixel();
		m_roi.bytesPerRow = m_buffers->GetPitch();
		
		m_pixelMem = TAlloc<PixelMem>(m_roi.bytesPerRow * m_roi.height, e16ByteAlign);
	}
	return m_opened;
}

bool GenieCard::Close()
{
	if(m_opened)
	{
		if(m_living)
		{
			StopLive();								//corresponding StartLive()
		}
		DeleteObjects();
		m_opened = false;
	}
	return !m_opened;
}

size_t GenieCard::FrameInterval( )
{
	return size_t(m_stampInterval);
}

bool GenieCard::GrabTimeout(size_t ms)
{
	m_grabTimeout = ms;
	return true;
}

bool GenieCard::SetGrabMode(EnumGrabMode eMode)
{
	bool success = false;
	switch(eMode)
	{
	case FREERUN:

		break;
	case SOFTTRG:

		break;
	case HARDTRG:
		
		break;
	default:
		;
	}
	return success;
}

bool GenieCard::StartLive()
{
	if(m_opened && !m_living)
	{
		if( m_xfer->Grab())
		{	m_living = true;	}
	}
	return m_living;
}

bool GenieCard::StopLive()
{
	if(m_living)
	{
		m_living = false;
		m_xfer->Freeze();
	}
	return !m_living;
}

TAlloc<TImageCard::PixelMem> GenieCard::GetPixelMem()
{
	if(m_grabedEvent.Wait(m_grabTimeout))
	{
		m_grabedEvent.ResetEvent();
		return m_pixelMem;
	}
	else
	{
		return m_pixelMem.NullObject();
	}
}

void GenieCard::XferCallback(SapXferCallbackInfo *pInfo)
{
	static TTimeDiff td;
	GenieCard *pThis = (GenieCard *) pInfo->GetContext();
	if(!pThis->IsLiving()) return;
	
	// If grabbing in trash buffer, do not display the image, update the
	// appropriate number of frames on the status bar instead
	if (pInfo->IsTrash())
	{	}
	else
	{
		pThis->m_stampInterval = td.msec();
		td.Reset();

		void *pData = 0;
		int i = pThis->m_buffers->GetIndex();
		pThis->m_buffers->GetAddress(i,&pData);
		pThis->m_pixelMemBase = pData;

		pThis->m_grabedEvent.SetEvent();
		
		pThis->m_imageSignal.PulseEvent();
		//调用注册的处理函数或设置事件
// 		if(pThis->RegisteredFunc)
// 		{
// 			pThis->RegisteredFunc(pThis->m_regFuncParam);
// 		}
// 		else
// 		{
// 			memcpy(pThis->m_pixelMem.Base(), pData, pThis->m_pixelMem.Size());
// 			pThis->m_grabedEvent.SetEvent();
// 		}
	}
}

void* GenieCard::GetPixelMemBase() const
{
	return m_pixelMemBase;
}

bool GenieCard::RegisterProcFunc(REGCallbackFunc* func, void *param)
{
	RegisteredFunc = func;
	m_regFuncParam = param;
	return true;
}

bool GenieCard::SetROI(	size_t startX,
						 size_t startY,
						 size_t width,
						 size_t height)
{
	bool success = false;
	if(m_opened)
	{
		m_acqDevice->SetUpdateFeatureMode(SapAcqDevice::UpdateFeatureManual);
		m_acqDevice->SetFeatureValue("OffsetX", startX);
		m_acqDevice->SetFeatureValue("OffsetY", startY);
		m_acqDevice->SetFeatureValue("Width",   width);
		m_acqDevice->SetFeatureValue("Height",  height);
		m_acqDevice->UpdateFeaturesToDevice();
		m_acqDevice->SetUpdateFeatureMode(SapAcqDevice::UpdateFeatureAuto);
		success = true;
	}
	return success;
}

bool GenieCard::SetExpTime(size_t expTime)
{
	return (m_opened && (m_acqDevice->SetFeatureValue("ExposureTime", expTime)));
}

bool GenieCard::SetGain(size_t gain)
{
	return (m_opened && (m_acqDevice->SetFeatureValue("Gain", gain)));
}

bool GenieCard::GetROI	(size_t &startX,
						size_t &startY,
						size_t &width,
						size_t &height)
{
	bool success = false;
	if(m_opened)
	{
		UINT32 gnStartX =0, gnStartY =0, gnWidth =0, gnHeight =0;
		m_acqDevice->GetFeatureValue("OffsetX", &gnStartX);
		m_acqDevice->GetFeatureValue("OffsetY", &gnStartY); 
		m_acqDevice->GetFeatureValue("Width",	&gnWidth);
		m_acqDevice->GetFeatureValue("Height",	&gnHeight);
		startX = gnStartX;
		startY = gnStartY;
		width  = gnWidth;
		height = gnHeight;
		success = true;
	}
	return success;
}

bool GenieCard::GetExpTime (size_t &expTime)
{
	bool success = false;
	if(m_opened)
	{
		UINT32 gnExpTime =0;
		m_acqDevice->GetFeatureValue("ExposureTime", &gnExpTime);
		expTime = gnExpTime;
		success = true;		
	}
	return success;
}

bool GenieCard::GetGain	(size_t &gain)
{
	bool success = false;
	if(m_opened)
	{
		UINT32 gnGain =0;
		m_acqDevice->GetFeatureValue("Gain", &gnGain);
		gain = gnGain;
		success = true;		
	}
	return success;
}

bool GenieCard::SaveSetting()
{
	const char * pConfig = m_acqDevice->GetConfigFile();
	BOOL ok = m_acqDevice->SaveFeatures(pConfig);
	return ok != FALSE;
}

bool GenieCard::Open()
{
	return Open(NULL);
}


