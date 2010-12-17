#ifndef VALVE_H
#define VALVE_H

#include "TCritSect.h"
#include <windows.h>
#include "TypeDefine.h"
 
class Valve
{
public:
	typedef enum { eShortInterval = 1,      eLongInterval =3 }EIntervalType;
	static Valve& TheValve();
	~Valve();
	void			Enable		(bool enable);
	bool			IsEnable	();
	
	void			SetParam	(	const ValveParam&       param);
	ValveParam		GetParam	() const;


	bool			PushResult	(	EIndex             testIndex);

	unsigned int	CtrlResult	(   EIndex	        testIndex);


	void			ShortInterval(  EIndex             testIndex,
									unsigned int	        tempResult);
	void			LongInterval(   EIndex             testIndex,
					unsigned int	        tempResult);
private:
	bool            Control		(	EIndex             testIndex,
									unsigned int	        result);
	EIndex			TheOtherTest(   EIndex             testIndex);

private:
	Valve();
	Valve(const Valve& src);
	Valve& operator=(const Valve& src);

protected:
	int					m_resultInterval;
	bool		        m_enable;
private:
	enum	{ CAMCOUNT  = 2, STACKLEN    = 256 };
	unsigned int            m_resultStack[STACKLEN];
	EIndex	                m_pushedIndex;

	unsigned int            m_finalResult[CAMCOUNT];
	unsigned int            m_preResullt[CAMCOUNT];
};

#endif //VALVE_H