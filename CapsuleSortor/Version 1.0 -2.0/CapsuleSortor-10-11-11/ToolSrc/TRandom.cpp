#include "TRandom.h"
TRandom::TRandom(unsigned int seed)
: m_seed(seed),
  DistriFun(0),
  m_xMin(0.0),
  m_xMax(0.0),
  m_yMin(0.0),
  m_yMax(0.0)
{ }

int TRandom::CRandFun() const
{
	return(((m_seed = m_seed * 214013L + 2531011L) >> 16) & RANDMAX);
}

void	TRandom::SetSeed (unsigned int seed)
{
	m_seed = static_cast<unsigned long>(seed);
}

int TRandom::Uniform(int minVal, int maxVal) const
{
	return TToInteger::Round(Uniform(static_cast<double>(minVal), static_cast<double>(maxVal)));
}

double TRandom::Uniform(double minVal, double maxVal) const
{
	return minVal + CRandFun()*(maxVal-minVal)/(RANDMAX);
}

double TRandom::Gaussian(double mean, double deviation) const
{
	static bool		isNext			= false;
	static double	nextGaussianVal = 0.0;
	double gaussVal, fac, rsq, v1, v2;
	
	if (!isNext)
	{
		do
		{
			v1 = 2.0*Uniform(0.0, 1.0) - 1.0;
			v2 = 2.0*Uniform(0.0, 1.0) - 1.0;
			rsq = v1*v1 + v2*v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		
		fac = sqrt(-2.0*log( rsq)/rsq);
		nextGaussianVal = v1 * fac;
		gaussVal		= v2 * fac;
		isNext			= true;
	}
	else
	{
		gaussVal = nextGaussianVal;
		isNext		= false;
	}
	return mean + deviation * gaussVal;
}

bool TRandom::AssignDistribution(DistriFunc* func, double xMin, double xMax)
{
	// Initialises random generator for first call
	if (func != DistriFun)
	{
		DistriFun = func;
		// Evaluates maximum and minimum of function
		if(DistriFun != 0)
		{
			m_xMin = xMin;
			m_xMax = xMax;
			m_yMin = 0.0;
			m_yMax = DistriFun(xMin);
			for(int iX = 1; iX <= RANDMAX; ++iX)
			{
				double x = m_xMin + (m_xMax - m_xMin) * iX / RANDMAX;
				double y = DistriFun(x);
				m_yMax	 = m_yMax < y ? y : m_yMax; 
			}
		}
	}
	return 0 != func;
}

double TRandom::RandAsDistribution( ) const
{
	if(0 == DistriFun)	{	return 0.0;	}

	// Gets random values for X & Y
	double x = Uniform(m_xMin, m_xMax);
	double y = Uniform(m_yMin, m_yMax);

	// Returns if valid and try again if not valid
	return y < DistriFun(x) ? x : RandAsDistribution();
}
