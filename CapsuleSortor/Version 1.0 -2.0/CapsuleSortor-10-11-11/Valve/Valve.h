#ifndef VALVE_H
#define VALVE_H

#include "TCritSect.h"
#include <windows.h>
#include "TypeDefine.h"
 
class Valve
{
public:
	typedef enum { eFirst = 0,          eSecond = 1  } ETIndex;
	typedef enum { eShortInterval = 1,      eLongInterval =3 }EIntervalType;
	static Valve& TheValve();
	~Valve();
	void Enable			(bool enable);
	bool IsEnable		();
	
	void    SetParam	(       const ValveParam&       param);
	ValveParam GetParam	() const;


	bool    PushResult	(       ETIndex             testIndex);

	unsigned int CtrlResult	(       ETIndex	        testIndex);


	void	ShortInterval   (       ETIndex             testIndex,
					unsigned int	        tempResult);
	void	LongInterval    (       ETIndex             testIndex,
					unsigned int	        tempResult);
private:
	bool            Control	(	ETIndex             testIndex,
					unsigned int	        result);
	ETIndex TheOtherTest    (       ETIndex             testIndex);

private:
	Valve();
	Valve(const Valve& src);
	Valve& operator=(const Valve& src);

protected:
	int			m_resultInterval;
	bool		        m_enable;
	bool		        m_ioBdEnable;

private:
	enum		        { CAMCOUNT  = 2, STACKLEN    = 256 };
	unsigned int            m_resultStack[STACKLEN];
	ETIndex	                m_pushedIndex;
	TCritSect	        m_critSect;
	size_t		        m_stamp;

	unsigned int            m_finalResult[CAMCOUNT];
	unsigned int            m_preResullt[CAMCOUNT];
};

#endif //VALVE_H