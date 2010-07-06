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
	size_t		tolerance;			//最大误差
	size_t		width;				//胶囊的宽度
	size_t		height;				//胶囊的长度
};

struct CapsuleParam 
{
	CapsuleDim	capsuleDim;
	PelLab24	capColor;			//帽部分颜色阈值
	PelLab24	bodyColor;			//帽部分色度的容差值
	size_t		bodyLength;			//体高度
	size_t		capLength;			//帽高度
	size_t		partTolerance;		//部分高度的容差值
};

struct ValveParam
{
	int		IOBoard;
	int		ResultInterval;		//两个相机的视场间隔
};

struct SortorParam
{
	static int	transparent;		//透明处理标志	
	size_t		maxValue;			//胶囊最大亮度

	size_t		shrinkX;			//横向缩进值
	size_t		shrinkY;			//纵向缩进值

	size_t		edgeThres;			//灰度图像套和区阈值
	size_t		edgeDensity;		//灰度图像边沿分析密度
	size_t		dynThres;			//动态阈值的阈值	
	size_t		dynWndSize;			//动态阈值的窗口大小
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
	size_t			roiStartX;				//感兴趣区域起点
	size_t			roiStartY;						
	size_t			roiWidth;				//感兴趣区域尺寸		
	size_t			roiHeight;
	size_t			gain;					//增益
	size_t			expTime;				//曝光时间
	size_t			balanceRed;				//白平衡	
	size_t			balanceGreen;
	size_t			balanceBlue;
	std::string		camTag;					//相机标识

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
	size_t	radius;						//胶囊半径（像素单位）
	size_t	minBoxWidth;				//minbox宽度
	size_t	binaryLowThres;				//双峰二值化下边界
	size_t	binaryUpThres;				//双峰二值化上边界

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
