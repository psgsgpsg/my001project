// TSeriesComm.h: interface for the TSeriesComm class.

#ifndef CTRLCARDCOMM_H
#define CTRLCARDCOMM_H

#include "TSeriesComm.h"
#include "typeDefine.h"

class CtrlCardComm
{
public:
	enum LIGHTCOLOR
	{	
		RED		= 1,
		GREEN	= 2,
		YELLOW	= 3,
		CLOSEALL = 5
	};
	typedef union U_COMMCODE {	
		unsigned long code;	
		unsigned char data[4];
		U_COMMCODE(unsigned long val=0)
		{	code = val;		}
		U_COMMCODE(	unsigned char data0, unsigned char data1, 
					unsigned char data2, unsigned char data3)
		{
			data[0] = data0;	data[1] = data1;
			data[2] = data2;	data[3] = data3;
		}
	} COMMCODE;
	typedef union U_COMMDATA {
		struct {
			unsigned char low;
			unsigned char high;
		};
		unsigned short data;
		U_COMMDATA(unsigned short val=0)
		{	data = val;		}
		U_COMMDATA(unsigned char highByte, unsigned char lowByte) 
		{	high = highByte; low = lowByte;	}
	} COMMDATA;

	~CtrlCardComm();
	static CtrlCardComm& TheComm();

public:
	bool Open	(size_t portIndex, const size_t baudRate);
	bool Open	();

	void Close	( );

	//���Ź�
	bool WatchDog(	bool enble);

	//������
	bool SenserCfg( unsigned char	index,
					unsigned short	noWorkTime);

	//����ģʽ
	bool CountMode(	unsigned char	mode /* 0x01���������� 0x02��������*/);

	//��� && �����
	bool PulseSet(	unsigned char	index,
					unsigned short	camDelay,
					unsigned short	camHold,
					unsigned short	lightDelay,
					unsigned short	ligntHold);
	//
	bool SetParam( const	ComCtrlParam & comParam);

	//��ʹ��״̬
	bool LampSet(	unsigned char	index,
					bool			enable);/*true ������ false ���� */

	//��ŷ�
	bool ValveCfg(	unsigned char  index, 
					unsigned short delayTime, 
					unsigned short holdTime);

	//�򿪵�ŷ�
	bool ValveTest(	unsigned char  index);

	//�Ƹ�
	bool LightCtrl( unsigned char  lampIndex, 
					bool flashing);

	//����ҩ��
	bool SendBadInfo	(	unsigned char camIndex,
							unsigned char capIndex);

	//�������������
	bool CamValveDistance(	unsigned char	camIndex, 
							unsigned short	distance);

	//ҩ����
	bool CapsuleInterval(	unsigned short	Interval);

	//��뾶
	bool AxisRadius		(	unsigned short	radius);

	//������������
	bool PulseInterval	(	unsigned short	Interval);

	//��ת����������
	bool RotCoderCfg	(	unsigned short precision,
							unsigned short adjustVal);

	//��շ��ͻ�����
	bool ClearTxBuff	();
public:
	COMMCODE CamDelayCode	(unsigned char index);
	COMMCODE CamHoldCode	(unsigned char index);
	COMMCODE LampDelayCode	(unsigned char index);
	COMMCODE LampHoldCode	(unsigned char index);
	COMMCODE SenserCfgCode	(unsigned char index);
	COMMCODE ValveCfgCode	(unsigned char index);
	COMMCODE CamValveDistCode(unsigned char index);	
	COMMCODE WatchDogCode			();
	COMMCODE CountModeCode			();
	COMMCODE CapsuleIntervalCode	();
	COMMCODE AxisRadiusCode			();
	COMMCODE PulseIntervalCode		();
	COMMCODE RotCoderPrecisionCode	();
	COMMCODE RotCoderAdjustValCode	();

	COMMCODE LampFlagCode	();
	
	COMMDATA GetCurState(const COMMCODE& code);
	bool	 GetCurState( );
	bool	 SetState(const COMMCODE& code, const COMMDATA& data);

	bool	 ClearRxBuffer();
	unsigned char * GetRxBuffer(size_t &buffSize);
private:
	
	bool BuildData(	unsigned char	data1,
					unsigned char	data2,
					unsigned char	data3,
					unsigned char	data4,
					unsigned char	data5,
					unsigned char	build[8]);

	bool BuildData(	const COMMCODE& code, 
					const COMMDATA& data, 
					unsigned char	build[8]);
private:
	CtrlCardComm();
	CtrlCardComm(const CtrlCardComm& src);
	CtrlCardComm& operator = (const CtrlCardComm& src);

private:
	static const size_t	SENDNUM;
	const size_t		READNUM;
	unsigned char*		m_pReadData;
	TSeriesComm			m_comm;
	size_t				m_portIndex;
	size_t				m_baudRate;
};

#endif // CTRLCARDCOMM_H