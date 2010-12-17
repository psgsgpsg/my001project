#ifndef CAPSULEPROC_H
#define CAPSULEPROC_H

#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include "iCVCImg.h"
#include "iCVFoundation.h"
#include "iCVCEdge.h"
#include "iLightmeter.h"
#include <vector>
#include <string>

#include "TRemain.h"
#include "TTransRemain.h"
#include "TAllocTmpl.h"
#include "TImgProcess.h"
#include "TImgBuffer.h"
#include "TRect2D.h"

#include "TypeDefine.h"
#include "SortObserver.h"
#include "SortResult.h"
#include "TColorChecker.h"
#include "valve.h"

class CapsuleProc
{
public:
	typedef enum
	{ REALTIME,	TRAIN	} WORKMODE;
	/*enum { SUCCESS = 0, SUBIMGNUM = 4 };*/
	enum	{ CAPSULENUM = 4,	BAD_ARRAY_LEN = 10};
	typedef enum 
	{ eNoSave = 0, eErrSave, eAllSave } ESaveState;
	CapsuleProc();
	virtual ~CapsuleProc();

public:
	virtual bool			ReInit			(	const TImgDim& imgDimension) = 0;
	virtual unsigned int	Process			( WORKMODE mode = REALTIME) = 0;
	virtual TCapsuleParam	GetProcedureData() = 0;	

	const SortorParam&		SegmentParam	        ( ) const;
	const RemainParam&		RemainInfo		( ) const;
	const TImgDim&			Dimension		( ) const;
        const CapsuleParam&		CapsuleInfo		( );
	
	void		CapsuleInfo		(       const CapsuleParam& param);
	void		SegmentParam	        (	const SortorParam& param);
	void		RemainInfo		(	const RemainParam& param);
	bool		SetRawImage		(	void*	pMem, 	size_t	bytes);
	IMG		GetRawImage		(               );
	int		CheckPosition	        (	long    posX, 
                                                        long    posY);
	bool		IsCapsuleShort	        (	size_t  realHeight,
							size_t  realWidth,
							size_t  stdHeight,
							size_t  tolerance);
	bool	        FindOverlapCenter       (	IMG	rawSubImg,
							long&	centerX,
							long&	centerY);
	long		FindFirstEdgeY	        (	IMG     image, 
							TArea   area, 
							long    density, 
							double  threshold);
	bool		FindUpDownEdge	        (	IMG     image, 
							long    density,
							double  threshold, 
							long    &upPos, 
							long    &downPos	);

	TArea		CreateTArea		(	long    orgX,
							long    orgY, 
							long    width, 
							long    height		);
	bool		ShrinkHorizontal        (	IMG     image,
							const size_t	shrinkNum);

	bool		ShrinkVertical	        (	IMG	image, 
							const size_t	shrinkNum);

	bool		RotateIMG		(	size_t	blobIndex,
							FBLOB	profileBlob,
							IMG	proImg,
							IMG	&proSubIMG,
							size_t	&posIndex);

	bool		RectangleBlob	        (	IMG	image,
							const TRect2D<long>&	roi,
							long	wndSize,
							long	dynThres,
							long	minBlobSize,
							TBlobTouchBorderFilter	ignorBorder		= FBLOB_BORDER_ALL,
							long	minBlobWidth	= -1,
							long	minBlobHeight	= -1,
							bool	alsoReverse	= false);

	bool		RectSobelBlob	        (	IMG	image, 
							const TRect2D<long>&	roi,
							long	wndSize,
							long	dynThres,
							long	minBlobSize,
							TBlobTouchBorderFilter	ignorBorder		= FBLOB_BORDER_NONE,
							long	minBlobWidth	= -1,
							long	minBlobHeight	= -1);	


	bool		RectBlackBlob	        (	IMG	image,
							long	wndSize,
							long	dynThres,
							long	minBlobSize,
							TBlobTouchBorderFilter	ignorBorder		= FBLOB_BORDER_ALL,
							long	minBlobWidth	= -1,
							long	minBlobHeight	= -1);


	bool		BlobAnalysis	        (	IMG	image,
							long    upEdgeY, 
							long    downEdgeY,
							long	wndSize,
							long	dynThres,
							long	blobSize);

	bool		SobelAnalysis	        (	IMG	image);

	bool		ProfileBlob		(	IMG	image, 
							long	minWidth,
							FBLOB   &blob		);

	bool		TransSobelAnalysis	(	IMG	image);
	bool		TransRotateIMG		(	size_t	blobIndex,
							FBLOB	profileBlob,
							IMG	proImg,
							IMG	&proSubIMG,
							size_t	&posIndex);
	bool		TransDoubleJudge(       IMG	&subImage,
							const size_t	radius);
	bool		TransIsPartShort(	const size_t height,
							const size_t upEdge,
							const size_t downEdge);
	bool		TransIsShort(	const size_t realSize,
							const size_t stdSize1,
							const size_t stdSize2,
							const size_t tolerance);
	bool		TransBlobAnalysis(	IMG	image,
								long    upEdgeY, 
								long    downEdgeY,
								long	wndSize,
								long	dynThres,
								long	blobSize);
	const	EIndex	GetProcessIndex()	{	return m_processIndex;	};

	
public:
	static IMG		ObservedIMG		( );
	static bool		ObservedIMG( SortObserver::ETarget eTarget, IMG img );
	static double		ObservedParam	( );
	static void		ObservingTarget	(	unsigned int spec);
	static void		AddToAllCount	(	size_t num ) { m_allCount += num;	}
	static void		AddToBadCount	(	size_t num ) { m_badCount += num;	}
	static void		ClearCount	( ) { m_allCount = m_badCount = 0;		}
	static size_t		AllCount	( ) { return m_allCount;	}
	static size_t		BadCount	( ) { return m_badCount;				}
	static ESaveState	GetSaveState	( );
	static ESaveState	SetSaveState	(	ESaveState NextState);

	void			SetProcessIndex	(	EIndex index){	m_processIndex = index;};

	TImgBuffer		GetRecordedImage( );
	void			ClearRecordedImage();
	unsigned int		GetResultPos	( )	{ return m_sortResult.GetErrorPosition();	}
	
	unsigned int		TransProcess	( WORKMODE mode);
protected:
	bool			RecordingImage	( );

	TCapsuleParam		m_curData;

protected:
	TAlloc<PixelMem>	m_rawImage;							//ԭʼͼ����
	IMG					m_cvbImg;							//����ͼ����
	FBLOB				m_cvbBlob;

	TImage<PelGray8>	m_profileImage;						//����ͼ���ڴ�
	IMG					m_cvbProfileImg;					//����ͼ����
	FBLOB				m_profileBlob;
	LMH					m_lightMeter;						//lighterMeter������

	TRemain				m_remainTool;
	TTransRemain		m_transRemain;						//͸��������ȡ
	RemainParam			m_remainParam;						//��ȡ����

	SortorParam			m_segmentParam;						//��ȡģ�����	

	SortResult			m_sortResult;
	static ESaveState	m_eSaveState;
	static SortObserver	m_sortObserver;
	CapsuleParam		m_capsuleParam;						//ģ�����ݶ���

private:
	TImgBuffer		m_imgBuffer;
	TImgDim			m_dim;
	EIndex	    m_processIndex;
	static size_t		m_allCount;							//ȫ����������
	static size_t		m_badCount;							//��Ʒ��������
	//int				m_badArray[CAPSULENUM][BAD_ARRAY_LEN];
};

#endif //CAPSULEPROC_H