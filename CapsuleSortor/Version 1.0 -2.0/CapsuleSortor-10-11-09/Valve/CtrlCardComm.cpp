// CtrlCardComm.h: interface for the CtrlCardComm class.

#include "CtrlCardComm.h"

const size_t CtrlCardComm::SENDNUM = 8;
CtrlCardComm::CtrlCardComm()
: READNUM(512),
  m_pReadData(0)
{
	m_pReadData = new unsigned char[READNUM];
}

CtrlCardComm::~CtrlCardComm()
{
	Close();
	delete []m_pReadData;
}

CtrlCardComm& CtrlCardComm::TheComm()
{
	static CtrlCardComm theComm;
	return theComm;
}
bool CtrlCardComm::Open(size_t portIndex, const size_t baudRate)
{
	bool success = false;
	if( m_comm.Open(portIndex) )
	{
		m_comm.Config(baudRate, 0, 8, 0);
		success = true;
	}
	return success;
}
bool CtrlCardComm::Open	()
{
	bool success = false;
	if (Open( m_portIndex, m_baudRate))
	{
		success = true;
	}
	return success;
}

void CtrlCardComm::Close()
{
	m_comm.Close();
}

bool CtrlCardComm::WatchDog(	bool enble)
{
	COMMDATA data;
	data.low = 0x00;
	data.high = enble ? 0x01 : 0x00;
	return SetState(WatchDogCode(), data);
}

bool CtrlCardComm::CountMode(unsigned char	mode /* 0x01£º±àÂëÆ÷£¬ 0x02£º´«¸ÐÆ÷*/)
{
	bool success = false;
	if(mode == 0x01 || mode == 0x02)
	{
		success = SetState(CountModeCode(), COMMDATA(mode, 0x00));
	}
	return success;
}
bool CtrlCardComm::SenserCfg(	unsigned char	index,
								unsigned short	noWorkTime)
{
	return SetState(SenserCfgCode(index), noWorkTime);
}

bool CtrlCardComm::PulseSet(unsigned char	index,
							unsigned short	camDelay,
							unsigned short	camHold,
							unsigned short	lampDelay,
							unsigned short	lampHold)
{
	bool success = false;
	if((index>0) && (index<=6))
	{
		SetState(CamDelayCode (index), camDelay);
		SetState(CamHoldCode  (index), camHold);
		Sleep(100);
		SetState(LampDelayCode(index), lampDelay);
		SetState(LampHoldCode(index), lampHold);
		success = true;
	}
	return success;
}

bool CtrlCardComm::BuildData(unsigned char	data1,
							unsigned char	data2,
							unsigned char	data3,
							unsigned char	data4,
							unsigned char	data5,
							unsigned char	build[8])
{
	build[0] = 0xA0;
	build[1] = data1;
	build[2] = data2;
	build[3] = data3;
	build[4] = data4;
	build[5] = data5;
	build[6] = 0xAF;
	build[7] = build[0];
	for(int i=1; i<7; ++i)
	{	build[7] = build[i] ^ build[7];	}

	return true;
}

bool CtrlCardComm::BuildData(const COMMCODE& code, 
							 const COMMDATA& data, 
							 unsigned char	build[8])
{
	BuildData(	code.data[1], 
				code.data[2], 
				code.data[3],
				data.high,
				data.low,
				build);
	return true;
}

bool CtrlCardComm::ValveCfg(unsigned char  index, 
							unsigned short delayTime, 
							unsigned short holdTime)
{
	return SetState(ValveCfgCode(index), holdTime);
}

bool CtrlCardComm::ValveTest(unsigned char index)
{
	unsigned char data[SENDNUM];
	BuildData(0x40, 0x02, 0x01, index, 0x00, data);
	m_comm.Write(data,	SENDNUM);
	return true;
}

bool CtrlCardComm::LightCtrl(unsigned char	lampIndex,
							 bool			flashing)
{
	unsigned char state = flashing ?  0x00 : 0x01;
	unsigned char data[SENDNUM];
	BuildData(0x50, 0x01, lampIndex, state, 0x00, data);
	m_comm.Write(data,	SENDNUM);
	return true;
}

bool CtrlCardComm::SendBadInfo(	unsigned char camIndex,
								unsigned char capIndex)
{
	unsigned char data[SENDNUM];
	BuildData(0x10, 0x02, 0x01, camIndex, capIndex, data);
	m_comm.Write(data,	SENDNUM);
	return true;
}

bool CtrlCardComm::CamValveDistance(unsigned char	camIndex, 
									unsigned short	distance)
{
	return SetState(CamValveDistCode(camIndex), distance);
}

bool CtrlCardComm::CapsuleInterval(	unsigned short	Interval)
{
	return SetState(CapsuleIntervalCode(), COMMDATA(Interval&0xFF, 0x00));
}

bool CtrlCardComm::AxisRadius(	unsigned short	radius)
{
	return SetState(AxisRadiusCode(), radius);
}

bool CtrlCardComm::PulseInterval(	unsigned short	Interval)
{
	return SetState(PulseIntervalCode(),  COMMDATA(Interval & 0xFF, 0x00));
}

bool CtrlCardComm::RotCoderCfg(	unsigned short precision,
								unsigned short adjustVal)
{
	SetState(RotCoderPrecisionCode(), precision);
	SetState(RotCoderAdjustValCode(), COMMDATA(adjustVal & 0xFF, 0x00));
	return true;
}

bool CtrlCardComm::ClearRxBuffer(	)
{
	return m_comm.ClearBuffer(TSeriesComm::eRxClear);
}

CtrlCardComm::COMMCODE CtrlCardComm::CamDelayCode(unsigned char index)
{
	COMMCODE code;
	code.data[0] = 0xA0;
	code.data[1] = 0x20;
	code.data[2] = (index/4)+1;
	code.data[3] = (index*4-2)%12;
	return code;
}

CtrlCardComm::COMMCODE CtrlCardComm::CamHoldCode(unsigned char index)
{
	COMMCODE code = CamDelayCode(index);
	code.data[3] += 1;
	return code;
}

CtrlCardComm::COMMCODE CtrlCardComm::LampDelayCode(unsigned char index)
{
	COMMCODE code = CamDelayCode(index);
	code.data[3] += 2;
	return code;
}

CtrlCardComm::COMMCODE CtrlCardComm::LampHoldCode(unsigned char index)
{
	COMMCODE code = CamDelayCode(index);
	code.data[3] += 3;
	return code;
}

CtrlCardComm::COMMCODE CtrlCardComm::CamValveDistCode(unsigned char index)
{
	return COMMCODE(0xA0, 0x30, 0x01, index);	
}

CtrlCardComm::COMMCODE CtrlCardComm::SenserCfgCode(unsigned char index)
{
	return COMMCODE(0xA0, 0x20, index, 0x01);	
}

CtrlCardComm::COMMCODE CtrlCardComm::ValveCfgCode(unsigned char index)
{
	return COMMCODE(0xA0, 0x40, 0x01, index);		
}
CtrlCardComm::COMMCODE CtrlCardComm::CapsuleIntervalCode()
{
	return COMMCODE(0xA0, 0x30, 0x02, 0x01);
}
CtrlCardComm::COMMCODE CtrlCardComm::AxisRadiusCode	()
{
	return COMMCODE(0xA0, 0x30, 0x02, 0x02);	
}
CtrlCardComm::COMMCODE CtrlCardComm::RotCoderPrecisionCode()
{
	return COMMCODE(0xA0, 0x30, 0x02, 0x03);
}
CtrlCardComm::COMMCODE CtrlCardComm::PulseIntervalCode()
{
	return COMMCODE(0xA0, 0x30, 0x02, 0x04);	
}

CtrlCardComm::COMMCODE CtrlCardComm::RotCoderAdjustValCode()
{
	return COMMCODE(0xA0, 0x30, 0x03, 0x01);
}

CtrlCardComm::COMMCODE CtrlCardComm::WatchDogCode()
{
	return COMMCODE(0xA0, 0x10, 0x01, 0x01);
}
CtrlCardComm::COMMCODE CtrlCardComm::CountModeCode()
{
	return COMMCODE(0xA0, 0x10, 0x02, 0x02);
}

bool CtrlCardComm::GetCurState( )
{
	ClearRxBuffer();
	memset(m_pReadData, 0, READNUM);

	unsigned char data[SENDNUM];
	BuildData(0x10, 0x03, 0x01, 0x00, 0x00, data);
	m_comm.Write(data,	SENDNUM);
	m_comm.FlushBuffer( );
	Sleep(200);
	m_comm.Read(m_pReadData, READNUM);
	Sleep(200);
	unsigned char flag[5] = {0xA0, 0xFF, 0x01, 0xAF, 0xA0};
	return (0 == memcmp(flag, m_pReadData, 5));
}

CtrlCardComm::COMMDATA CtrlCardComm::GetCurState(const COMMCODE& code)
{
	for(size_t index = 4; index < READNUM; index += SENDNUM)
	{
		if(code.code == (reinterpret_cast<COMMCODE*>(m_pReadData+index))->code)
		{
			COMMDATA data;
			data.high = *(m_pReadData+index + 4);
			data.low  = *(m_pReadData+index + 5);
			return	data;
		}
	}
	return -1;
}

bool CtrlCardComm::SetState(const COMMCODE& code, const COMMDATA& data)
{
	unsigned char sendData[SENDNUM];
	BuildData(code, data, sendData);
	m_comm.Write(sendData, SENDNUM);
	return true;
}

bool CtrlCardComm::SetParam(const	ComCtrlParam & comParam)
{
	m_portIndex = comParam.portIndex;
	m_baudRate = comParam.baudRate;
	return true;
}

CtrlCardComm::COMMCODE CtrlCardComm::LampFlagCode	()
{
	return COMMCODE(0xA0, 0x10, 0x04, 0x01);
}

bool CtrlCardComm::LampSet(	unsigned char	index,
							bool			enable)
{
	COMMDATA	data;
	data.high = index;
	data.low = enable ? 0x00:0x01;
	SetState(LampFlagCode(), data);
	return true;
}

unsigned char * CtrlCardComm::GetRxBuffer(size_t &buffSize)
{
	if (!m_pReadData)
	{
		buffSize = 0;
		return NULL;
	}

	memset(m_pReadData, 0, READNUM);
	
	Sleep(200);
	m_comm.Read(m_pReadData, READNUM);
	Sleep(200);

	buffSize = READNUM;
	return m_pReadData;
}

bool CtrlCardComm::ClearTxBuff	()
{
	return m_comm.ClearBuffer(TSeriesComm::eTxClear);
}