#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include "CapsuleProc.h"

class MonoProcess : public CapsuleProc
{
public:
	MonoProcess();
	~MonoProcess();

	virtual bool			ReInit	(const TImgDim& imgDimension);
	virtual unsigned int	Process	( WORKMODE mode = REALTIME);
	virtual TCapsuleParam	GetProcedureData();
	bool					IsPartShort	(	const size_t	height, 
													size_t			upEdge, 
													size_t			downEdge)const;
	unsigned int			ComProcess(WORKMODE mode);	

};

#endif //IMAGEPROCESS_H