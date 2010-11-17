#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

#include <string>

#include "PixelType.h"
#include "SimularCard.h"
//#include "ProciliCard.h"
//#include "GenieCard.h"
#include "svsgigecard.h"

typedef		SVSGigECard		FirstCam;
typedef		SVSGigECard		SecondCam;

class ColorProcess;
class MonoProcess;

#ifdef FIRST_COLOR
typedef		ColorProcess	FirstProcess;
typedef		PelRGB24		FirstCamPixel;
#else
typedef		MonoProcess		FirstProcess;
typedef		PelGray8		FirstCamPixel;
#endif

#ifdef SECOND_COLOR
typedef		ColorProcess	SecondProcess;
typedef		PelRGB24		SecondCamPixel;
#else
typedef		MonoProcess		SecondProcess;
typedef		PelGray8		SecondCamPixel;
#endif

struct CapsuleDim
{
	size_t		tolerance;			//������
	size_t		width;				//���ҵĿ��
	size_t		height;				//���ҵĳ���
};

struct CapsuleParam 
{
	CapsuleDim	capsuleDim;
	PelLab24	capColor;			//ñ������ɫ��ֵ
	PelLab24	bodyColor;			//ñ����ɫ�ȵ��ݲ�ֵ
	size_t		bodyLength;			//��߶�
	size_t		capLength;			//ñ�߶�
	size_t		partTolerance;		//���ָ߶ȵ��ݲ�ֵ
};

struct ValveParam
{
	int		IOBoard;
	int		ResultInterval;		//����������ӳ����
};

struct SortorParam
{
	static int	transparent;		//͸�������־	
	size_t		maxValue;			//�����������

	size_t		shrinkX;			//��������ֵ
	size_t		shrinkY;			//��������ֵ

	size_t		edgeThres;			//�Ҷ�ͼ���׺�����ֵ
	size_t		edgeDensity;		//�Ҷ�ͼ����ط����ܶ�
	size_t		dynThres;			//��̬��ֵ����ֵ	
	size_t		dynWndSize;			//��̬��ֵ�Ĵ��ڴ�С
	size_t		blobSize;

	SortorParam()
	:maxValue(230),
	shrinkX(5),
	shrinkY(5),
	edgeThres(3),
	edgeDensity(300),
	dynThres(10),
	dynWndSize(30),
	blobSize(20)
	{ }
};


struct CameraParam
{
	size_t			roiStartX;				//����Ȥ�������
	size_t			roiStartY;						
	size_t			roiWidth;				//����Ȥ����ߴ�		
	size_t			roiHeight;
	size_t			gain;					//����
	size_t			expTime;				//�ع�ʱ��
	size_t			balanceRed;				//��ƽ��	
	size_t			balanceGreen;
	size_t			balanceBlue;
	std::string		camTag;					//�����ʶ

	CameraParam() :
        balanceRed(100), 
	balanceGreen(100),
	balanceBlue(100)
	{ }
};

struct ComCtrlParam
{
	size_t	portIndex;
	size_t	baudRate;
ComCtrlParam():
	portIndex(1),
	baudRate(57600)
	{}
};

struct RadiumRange
{
	int		minRadium;
	int		maxRadium;
	RadiumRange():
	minRadium(40),
	maxRadium(70)
	{}
};

struct MinboxWidthRange 
{
	int		minWidth;
	int		maxWidth;
	MinboxWidthRange():
	minWidth(0),
	maxWidth(20)
	{}
};

struct RemainParam
{
	size_t	radius;						//���Ұ뾶�����ص�λ��
	size_t	minBoxWidth;				//minbox���
	size_t	binaryLowThres;				//˫���ֵ���±߽�
	size_t	binaryUpThres;				//˫���ֵ���ϱ߽�

	RemainParam():
	radius(60),
	minBoxWidth(15), 
	binaryLowThres(50),
	binaryUpThres(255)
	{}
};

class TCapsuleParam
{
	
private:
	CapsuleParam		m_info;
public:
	void	Clear()														{	memset(&m_info, 0x00, sizeof(CapsuleParam));	};
	void	WidthAndHeight(const size_t	width, const size_t height)		{	m_info.capsuleDim.width = width;;
																			m_info.capsuleDim.height = height;	};
	void	PartLength( const size_t part1, const size_t part2)			{	m_info.bodyLength	= part1 > part2?part1:part2;
																			m_info.capLength	= part1 > part2?part2:part1;	};
	void	BodyColor	(PelLab24	&bodyLab)							{	m_info.bodyColor = bodyLab;							};	
	void	CapColor	(PelLab24	&capLab)							{	m_info.capColor = capLab;						};
	CapsuleParam&		GetParam()										{	return m_info;									};

};


#endif //TYPEDEFINE_H
