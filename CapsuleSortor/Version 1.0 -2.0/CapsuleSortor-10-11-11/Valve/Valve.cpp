
#include "Valve.h"
#include "TLocker.h"
#include "CtrlCardComm.h"

Valve::Valve()
:   m_stamp(0),
   m_resultInterval(0),
  m_pushedIndex(eFirst),
  m_enable(true),
  m_ioBdEnable(true)
{
	memset			(m_resultStack, 0, sizeof(m_resultStack));
	memset(m_finalResult, 0, sizeof(m_finalResult));
	memset(m_preResullt, 0, sizeof(m_preResullt));
}

Valve::~Valve()
{
}

Valve& Valve::TheValve()
{
	static Valve theValve;
	return theValve;
}

bool Valve::IsEnable( )
{
	return m_enable;
}

void Valve::Enable(bool enable)
{
	m_enable = enable;
	Control(eFirst, 0x1);
}

/*******************************************************************************
*	�������ƣ�	Control
*	
*	����	��	result		������	
*				delayTime	�ӳ�ʱ��
*				period		����ͼ����Ҫ��ʱ��
*			  
*	���	��	�ɹ�	true
*				ʧ��	false
*
*
*	����	���������������Ϊ��Ӧ���ӳ�ʱ��
*
*
********************************************************************************/

bool Valve::Control(ETIndex testIndex,	unsigned int result)
{
	if(!m_enable)	return false;
	for(int i=0; i<4; i++)
	{
		if((result>>i) & 0x01)
		{			
			CtrlCardComm::TheComm().SendBadInfo(1, 4-i);
		}
	}
	return true;
}

Valve::ETIndex Valve::TheOtherTest (ETIndex testIndex)
{
	return (eFirst == testIndex) ? eSecond : eFirst;
}


/*******************************************************************************
*	�������ƣ�	PushResult
*	
*	����	��	testIndex	�������
*				result		������
*			  
*	���	��	�ɹ� true ʧ�� false
*
*
*	����	��������Ľ��ѹ�����
*
*
********************************************************************************/

bool Valve::PushResult(ETIndex testIndex)
{
	if (eSecond == testIndex)
	{
		for(int i = m_resultInterval; i >0; i--)
		{
			m_resultStack[i] = m_resultStack[i-1];
		}

		m_resultStack[0]				= m_finalResult[testIndex]&0xf;
        m_finalResult[testIndex]		= 0;
		m_pushedIndex = eSecond;
	}
	return true;
}

/*******************************************************************************
*	�������ƣ�	CtrlResult
*	
*	����	��	testIndex		�������
*				resultIndex		������
*				delayTime		�ӳ�ʱ��
*				period			�����������
*			  
*	���	��	��Ʒ������
*
*
*	����	��ִ�ж�ʱ����
*
*
********************************************************************************/

unsigned int  Valve::CtrlResult(ETIndex testIndex)
{
	unsigned int badSum	= 0;
	unsigned int finalResult	= 0;
	unsigned int stackValue		= m_resultStack[m_resultInterval]&0xf;
	unsigned int resultValue	= m_finalResult[testIndex]&0xf;

	if (testIndex == m_pushedIndex)
	{
		for(int i = m_resultInterval; i >0; i--)
		{
			m_resultStack[i] = m_resultStack[i-1];
		}
		
		m_resultStack[0] = 0;
	}
				
	finalResult = resultValue|((stackValue<<1)&0xf);
	finalResult = finalResult|((m_resultStack[m_resultInterval-1]>>3)&0xf);			
	
	Control(testIndex, finalResult);
	for( int i = 0; i < 4; ++i)
	{
		badSum +=((finalResult>>i)&0x1);
	}
	m_pushedIndex = eFirst;
	return badSum;	
}

void Valve::SetParam(const ValveParam& param)
{
	m_ioBdEnable	 = (param.IOBoard != 0);
	m_resultInterval = param.ResultInterval;
}

ValveParam Valve::GetParam	() const
{
	ValveParam param;
	param.IOBoard		= m_enable ? 1 : 0;
	param.ResultInterval= m_resultInterval;
	return param;
}

void	Valve::ShortInterval(ETIndex testIndex,	  unsigned int	tempResult)
{
	m_finalResult[testIndex] = (((m_preResullt[testIndex]<<1)&0xf)| (tempResult&0xf))&0xf;
}
void	Valve::LongInterval(ETIndex testIndex,	unsigned int	tempResult)
{
	m_finalResult[testIndex] = ((m_finalResult[testIndex]|((tempResult&0xf)>>3))&0xf);
	m_preResullt[testIndex] = (tempResult&0xf);
}