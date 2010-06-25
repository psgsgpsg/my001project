#ifndef VALVE_H
#define VALVE_H

#include "TCritSect.h"
#include <windows.h>
#include "TypeDefine.h"
#include "ConfigInfo.h"
 
class Valve
{
public:
	typedef enum { eFirst = 0,          eSecond = 1  } ETIndex;
	typedef enum { eShortInterval = 1,  eLongInterval =3 }EIntervalType;
	static Valve& TheValve();
	~Valve();
	void			Enable			(	bool enable);
	bool			IsEnable		();
	
	void			SetParam		(   const ValveParam&       param);
	ValveParam		GetParam		() const;


	bool			PushResult		(   ETIndex             testIndex,
										size_t				period);

	unsigned int	CtrlResultMode1		(   ETIndex	        testIndex, 
										size_t			period);
	unsigned int	CtrlResultMode2		(   ETIndex	        testIndex, 
										size_t			period);
	unsigned int	CtrlResult		(   ETIndex	        testIndex, 
										size_t			period);

	void			SetProcessResult(	ETIndex					index,
										unsigned int			procResult);
	void			SetResultMode1	(	ETIndex					index,
										unsigned int			procResult);
	void			ShortInterval   (   ETIndex					testIndex,
										unsigned int	        tempResult);
	void			LongInterval    (   ETIndex					testIndex,
										unsigned int	        tempResult);
	bool			PushResultMode2	(	ETIndex testIndex,
										size_t	period);
	bool			PushResultMode1	(	ETIndex testIndex,
										size_t	period);

	static void		AddToAllCount	(	size_t num ) 
	{ 
		GrabIndex indexMode = ConfigInfo::TheConfigInfo().GetGrabIndex();
		int i = 0;
		switch (indexMode)
		{
		case ONEBYONE:
			for ( i = 0; i <2; i++)
			{
				if ((num>>i)&&0x01)
				{
					m_allCount++;
				}
			}
			break;
		case TWOBYTWO:
			if (num&&0x01)
			{
				m_allCount++;
			}
			if (num&&0x04)
			{
				m_allCount++;
			}
			break;
		}	
	}
	static void		AddToBadCount	(	size_t num ) { m_badCount += num;	}
	static void		ClearCount		( ) {	m_allCount = 0;
											m_badCount = 0;		}
	static size_t	AllCount		( ) { return m_allCount;	}
	static size_t	BadCount		( ) { return m_badCount;				}
private:
	bool            Control			(	ETIndex             testIndex,
										unsigned int        result);
	ETIndex			TheOtherTest    (   ETIndex             testIndex);

private:
	Valve();
	Valve(const Valve& src);
	Valve& operator=(const Valve& src);

protected:
	int					m_resultInterval;
	bool		        m_enable;
	bool		        m_ioBdEnable;
	
private:
	enum		        { CAMCOUNT  = 2, STACKLEN    = 256 };
	EIntervalType		m_interType[CAMCOUNT];
	bool				m_excuteFlag[CAMCOUNT];
	unsigned int        m_resultStack[STACKLEN];
	ETIndex	            m_pushedIndex;
	TCritSect	        m_critSect;
	size_t		        m_stamp;

	unsigned int        m_finalResult[CAMCOUNT];
	unsigned int        m_preResullt[CAMCOUNT];
	static size_t		m_allCount;							//全部胶囊数量
	static size_t		m_badCount;							//次品胶囊数量
};

#endif //VALVE_H