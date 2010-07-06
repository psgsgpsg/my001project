// ProciliCard.cpp	: Interface of the ProciliCard class
//
// Copyright (c) 2006 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Module :ImageCard
// Create Date	: 2007.11.12
//
// A simple tool for Bitflow Image card. 

#include "ProciliCard.h"

ProciliCard::ProciliCard()
: m_grabTimeout(1000),
  m_hCamera (0),
  m_pixelBuffIndex(0),
  m_pixelMemBase (0),
  RegisteredFunc (0),
  m_regFuncParam (0),
  m_stampInterval(0),
  m_stampFrequency(1)
{
	PvInitialize();
}

ProciliCard::~ProciliCard()
{

	Close();	
	PvUnInitialize();
}

bool ProciliCard::SetCamTag(const char* camTag)
{
	if(camTag && strlen(camTag) < 32)
	{
		strcpy(m_camTag, camTag); 
	}
	else
	{
		memset(m_camTag, 0, 32);
	}
	return true;
}

bool ProciliCard::Open( const char * camTag)
{
	if(m_opened)	return m_opened;
	SetCamTag(camTag);

	//查询是否有相机, 最多等待 timeoutForWait 时间.
	while(0 == PvCameraCount())
	{
		static int  timeoutForWait = 3000;
		int			intervalForWait = 250;
		Sleep(intervalForWait);
		timeoutForWait -= intervalForWait;
		if(timeoutForWait  < 0)
		{
			return false;
		}
	}

	//打开相机
	tPvCameraInfo	cameraList[4]; 
	unsigned long	numCameras	= PvCameraList(cameraList, 4, NULL); 
	if(0 == numCameras)
	{
		return false;
	}

	int camIndex = -1;
	if(strlen(m_camTag) == 0)
	{
		camIndex = 0;
	}
	else
	{
		for(int i = 0; i<(int)numCameras; i++)
		{
			if(0 == strncmp (cameraList[i].SerialString, m_camTag, strlen(m_camTag)))
			{
				camIndex = i;
				break;
			}
		}
	}
	if( (camIndex>= 0) && (cameraList[camIndex].PermittedAccess & ePvAccessMaster))
	{
		m_opened = (ePvErrSuccess == PvCameraOpen(cameraList[camIndex].UniqueId, ePvAccessMaster, &m_hCamera)); 
	}

	//读取相机参数,并配置相关资源
	if(m_opened)
	{
		tPvUint32 width		= 0;
		tPvUint32 height	= 0;
		tPvUint32 allBytes	= 0;

		PvAttrUint32Get(m_hCamera, "TimeStampFrequency",	&m_stampFrequency);		
		PvAttrUint32Get(m_hCamera, "Width",					&width);
		PvAttrUint32Get(m_hCamera, "Height",				&height);
		PvAttrUint32Get(m_hCamera, "TotalBytesPerFrame",	&allBytes);

		m_roi.width			= width;
		m_roi.height		= height;
		m_roi.bytesPerPixel = allBytes / (width * height);
		m_roi.bitsPerPixel	= m_roi.bytesPerPixel * 8;
		m_roi.bytesPerRow	= m_roi.width * m_roi.bytesPerPixel;

		for(int buffIndex = 0; buffIndex < CAMBUFFCOUNT; ++buffIndex)
		{
			m_pixleBuff[buffIndex] = TAlloc<PixelMem>(allBytes, e16ByteAlign);
			m_frameInfo[buffIndex].Context[0]		= this;
			m_frameInfo[buffIndex].Context[1]		= (void*)(buffIndex);
			m_frameInfo[buffIndex].ImageBuffer		= m_pixleBuff[buffIndex].Base();
			m_frameInfo[buffIndex].ImageBufferSize	= allBytes;
		}
		
		tPvErr errcode = PvCaptureStart(m_hCamera);
		if(ePvErrSuccess == errcode)
		{
			PvAttrEnumSet(m_hCamera,	"AcquisitionMode",			"Continuous");
		}
	}
	return m_opened;
}

bool ProciliCard::Close()
{
	if(m_opened)
	{
		StopLive();	
		PvCaptureQueueClear(m_hCamera);	
		PvCaptureEnd(m_hCamera); 
		PvCameraClose(m_hCamera);
		m_hCamera	= NULL;
		m_opened	= false;
	}
	return !m_opened;
}

bool ProciliCard::StartLive()
{
	if(m_opened && !m_living)
	{
		if(ePvErrSuccess == PvCommandRun(m_hCamera, "AcquisitionStart"))
		{
			for(int frameIndex=0; frameIndex < CAMBUFFCOUNT; ++frameIndex)
			{
				PvCaptureQueueFrame(m_hCamera, &(m_frameInfo[frameIndex]), ProciliCard::FrameDoneCallBack);
			}
			m_living = true;
		}
	}
	return m_living;
}

bool ProciliCard::StopLive()
{
	if(m_living)
	{
		PvCommandRun(m_hCamera,	"AcquisitionStop");
		m_living = false;
	}
	return !m_living;
}


size_t ProciliCard::FrameInterval( )
{
	size_t interval = size_t(m_stampInterval*1000.0 / m_stampFrequency);
	return (interval < m_grabTimeout) ? interval : 0;
}

bool ProciliCard::GrabTimeout(size_t ms)
{
	m_grabTimeout = ms;
	return true;
}

bool ProciliCard::SetGrabMode(EnumGrabMode eMode)
{
	bool success = false;
	switch(eMode)
	{
	case FREERUN:
		strcpy(m_grabMode, "Freerun");
		success = true;
		break;
	case SOFTTRG:
		strcpy(m_grabMode, "Software");
		success = true;
		break;
	case HARDTRG:
		strcpy(m_grabMode, "Freerun");
		break;
	default:
		strcpy(m_grabMode, "Freerun");
	}
	return success;
}


TAlloc<TImageCard::PixelMem> ProciliCard::GetPixelMem()
{
	if(m_grabedEvent.Wait(m_grabTimeout))
	{
		m_grabedEvent.ResetEvent();
		return m_pixleBuff[m_pixelBuffIndex];
	}
	else
	{
		return m_pixleBuff[m_pixelBuffIndex].NullObject();
	}
}

void __stdcall ProciliCard::FrameDoneCallBack(tPvFrame* pFrame)
{
    // if the frame was completed we re-enqueue it
    if(pFrame->Status != ePvErrUnplugged && pFrame->Status != ePvErrCancelled)
	{
		ProciliCard* pThis = (ProciliCard*)(pFrame->Context[0]);
		if(!pThis->IsLiving()) return;

		//得到采集图像的首地址
		pThis->m_pixelMemBase	= pFrame->ImageBuffer;
		pThis->m_pixelBuffIndex	= size_t(pFrame->Context[1]);

		ULARGE_INTEGER	newStamp;
		newStamp.LowPart		= pFrame->TimestampLo;
		newStamp.HighPart		= pFrame->TimestampHi;
		pThis->m_stampInterval	= double(newStamp.QuadPart - pThis->m_oldStamp.QuadPart);
		pThis->m_oldStamp.QuadPart	= newStamp.QuadPart;

		//指定采集队列
        PvCaptureQueueFrame(pThis->m_hCamera, pFrame,	ProciliCard::FrameDoneCallBack);

		//调用注册的处理函数或设置事件
		if(pThis->RegisteredFunc)	{	pThis->RegisteredFunc(pThis->m_regFuncParam);	}
		else						{	pThis->m_grabedEvent.SetEvent();				}
	}
}

void* ProciliCard::GetPixelMemBase() const
{
	return m_pixelMemBase;
}

bool ProciliCard::RegisterProcFunc(REGCallbackFunc* func, void *param)
{
	RegisteredFunc = func;
	m_regFuncParam = param;
	return true;
}

bool ProciliCard::SetROI(	size_t startX,
							size_t startY,
							size_t width,
							size_t height)
{
	bool success = false;
	if(m_hCamera)
	{
		PvAttrUint32Set(m_hCamera, "RegionX", startX);
		PvAttrUint32Set(m_hCamera, "RegionY", startY); 
		PvAttrUint32Set(m_hCamera, "Width",   width);
		PvAttrUint32Set(m_hCamera, "Height",  height); 
		success = true;
	}
	if (success)
	{
		m_roi.width = width;
		m_roi.height = height;
		m_roi.bytesPerRow = m_roi.width * m_roi.bytesPerPixel;
	}
	return success;
}

bool ProciliCard::SetExpTime(size_t expTime)
{
	return (m_hCamera && (ePvErrSuccess == PvAttrUint32Set(m_hCamera, "ExposureValue", expTime)));
}

bool ProciliCard::SetGain(size_t gain)
{
	return (m_hCamera && (ePvErrSuccess == PvAttrUint32Set(m_hCamera, "GainValue", gain)));
}

bool ProciliCard::SetWhiteBal(	size_t redVal,
								size_t greenVal,
								size_t blueVal)
{
	bool success = false;
	if(m_hCamera)
	{
		PvAttrUint32Set(m_hCamera, "WhitebalValueRed",	redVal);
		PvAttrUint32Set(m_hCamera, "WhitebalValueBlue", blueVal);
		greenVal;
		success = true;
	}
	return success;
}


bool ProciliCard::GetROI(	size_t &startX,
							size_t &startY,
							size_t &width,
							size_t &height)
{
	bool success = false;
	if(m_hCamera)
	{
		tPvUint32 pvStartX =0, pvStartY =0, pvWidth =0, pvHeight =0;
		PvAttrUint32Get(m_hCamera, "RegionX", &pvStartX);
		PvAttrUint32Get(m_hCamera, "RegionY", &pvStartY); 
		PvAttrUint32Get(m_hCamera, "Width",   &pvWidth);
		PvAttrUint32Get(m_hCamera, "Height",  &pvHeight);
		startX = pvStartX;
		startY = pvStartY;
		width  = pvWidth;
		height = pvHeight;
		success = true;
	}
	return success;
}

bool ProciliCard::GetExpTime (size_t &expTime)
{
	bool success = false;
	if(m_hCamera)
	{
		tPvUint32 pvExpTime =0;
		PvAttrUint32Get(m_hCamera, "ExposureValue", &pvExpTime);
		expTime = pvExpTime;
		success = true;		
	}
	return success;
}

bool ProciliCard::GetGain	(size_t &gain)
{
	bool success = false;
	if(m_hCamera)
	{
		tPvUint32 pvGain =0;
		PvAttrUint32Get(m_hCamera, "GainValue", &pvGain);
		gain = pvGain;
		success = true;		
	}
	return success;
}

bool ProciliCard::GetWhiteBal(	size_t &redVal,
								size_t &greenVal,
								size_t &blueVal)
{
	bool success = false;
	if(m_hCamera)
	{
		tPvUint32 pvRVal =0, pvBVal = 0;
		PvAttrUint32Get(m_hCamera, "WhitebalValueRed",	&pvRVal);
		PvAttrUint32Get(m_hCamera, "WhitebalValueBlue", &pvBVal);

		redVal		= pvRVal;
		blueVal		= pvBVal;
		greenVal	= 100;
		success		= true;		
	}
	return success;
}

bool ProciliCard::SaveSetting()
{
	bool success = false;
	const char * powerUpFile = "3";
	
	 if(ePvErrSuccess == PvAttrEnumSet(m_hCamera, "ConfigFilePowerUp", powerUpFile ) )
	{
		if (ePvErrSuccess == PvAttrEnumSet(m_hCamera,"ConfigFileIndex", powerUpFile))
		{
			if (ePvErrSuccess == PvCommandRun(m_hCamera, "ConfigFileSave"))
			{
				success = true;
			}
		}
	}
	return success;
}

bool ProciliCard::Open	()
{
	return Open("02-2166A-06161");
}