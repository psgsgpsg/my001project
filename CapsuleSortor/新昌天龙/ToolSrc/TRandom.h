#ifndef TRANDOM_H
#define TRANDOM_H

#include <math.h>
#include "TToInteger.h"

class TRandom
{
public:
	enum	{ RANDMAX = 0x7fff };
	typedef double DistriFunc(double);	//DistributionFunc
	TRandom(unsigned int seed = 1);

public:
	void	SetSeed (unsigned int seed);
	double	Uniform	(double minVal,	double	maxVal) const;
	int		Uniform	(int	minVal = 0,	int	maxVal = TRandom::RANDMAX) const;
	double	Gaussian(double mean = 0.0,	double deviation = 1.0) const;

	bool	AssignDistribution(DistriFunc* func, double xMin, double xMax);
	double  RandAsDistribution( ) const;

private:
	int		CRandFun() const;

private:
	mutable unsigned long	m_seed;
	DistriFunc*				DistriFun;
	double					m_xMin;
	double					m_xMax;
	double					m_yMin;
	double					m_yMax;
};

#endif //TRANDOM_H
