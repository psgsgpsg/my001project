/****************************************************************************************
                     STEMMER IMAGING GmbH                       
-----------------------------------------------------------------------------------------

    Filename    : iCVFoundation.h

    Description : Standard header file for the
                  Common Vision Blox Foundation Package
    
    Created by  : Volker Gimple

    Revision    : 1.0
                  1.1   Added CreateDynamicThresholdImageEx and TDynamicThresholdNorm
                        (05.12.2006)

****************************************************************************************/


#ifndef INCLUDED_ICVFOUNDATION
#define INCLUDED_ICVFOUNDATION


// Borland C++ needs to be set so to use 4 bytes for enums!
// --------------------------------------------------------------------------------------
#if (defined __BCPLUSPLUS__) || (defined __BORLANDC__)
#pragma option -b
#endif


#include <iCVCImg.h>


// CVB object types
// --------------------------------------------------------------------------------------
typedef void * NLTRANSFORMATION;  // Nonlinear transformation object
typedef void * IMGMOMENTS;        // moments calculator object
typedef void * FBLOB;             // foundation package blob handle
typedef void * ANGLE;
typedef void * VECTOR2D;
typedef void * LINE2D;
typedef void * CIRCLE2D;


// Foundation DLL enumerations and Type definitions
// --------------------------------------------------------------------------------------
enum TThresholdComparison
{
  TH_Less           = 0,
  TH_LessOrEqual    = 1,
  TH_Equal          = 2,
  TH_GreaterOrEqual = 3,
  TH_Greater        = 4
};
enum TMorphologyMask
{
  MM_Custom         = 0,
  MM_Square         = 1,
  MM_Rect           = 2,
  MM_Cross          = 3,
  MM_Circle         = 4,
  MM_Ellipse        = 5
};
enum TFilterMask
{
  FM_3x3              = 0,
  FM_5x5              = 1,
  FM_7x7              = 2
};
enum TMomentsCalculation
{
  MC_FavorNone        = 0,
  MC_FavorSpeed       = 1,
  MC_FavorAccuracy    = 2
};
enum TInterpolationMode
{
  IP_NearestNeighbour = 0,
  IP_Linear           = 1,
  IP_Cubic            = 2,
  IP_Lanczos          = 3,
  IP_Supersample      = 4
};
enum TCannyEdgeFilter
{
  CE_Scharr           = 0,
  CE_Sobel            = 1,
  CE_Sobel2nd         = 2
};
enum TDistanceNorm
{
  DN_LInf             = 0,
  DN_L1               = 1,
  DN_L2               = 2
};
enum TBlobFeatureType
{
  FBLOB_BLACK_TO_FEATURE  = 0,
  FBLOB_WHITE_TO_FEATURE  = 1
};
enum TBlobTouchBorderFilter
{
  FBLOB_BORDER_NONE       = 0,
  FBLOB_BORDER_RIGHT      = 1,
  FBLOB_BORDER_TOP        = 2,
  FBLOB_BORDER_LEFT       = 4,
  FBLOB_BORDER_BOTTOM     = 8,
  FBLOB_BORDER_ALL        = 15
};
enum TBlobSortMode
{
  FBLOB_SORT_UNSORTED = 0,
  FBLOB_SORT_SIZE     = 1,
  FBLOB_SORT_POSX     = 2,
  FBLOB_SORT_POSY     = 3,
  FBLOB_SORT_DISTANCE = 4 
};
enum TBlobSortDirection
{
  FBLOB_SORT_RISING   = 1,
  FBLOB_SORT_FALLING  = 0
};
enum TDynamicThresholdNorm
{
  DTN_Mean            = 0,
  DTN_MinMax          = 1
};


// Nonlinear Transformation Functions
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               ApplyInverseNLTransform       ( NLTRANSFORMATION Transformation,
                                                            double &X, double &Y );
IMPORT(DWORD)               ApplyNLTransform              ( NLTRANSFORMATION Transformation,
                                                            double &X, double &Y );
IMPORT(DWORD)               ApplyNLTransformPixelList     ( NLTRANSFORMATION Transformation,
                                                            PIXELLIST List );
IMPORT(DWORD)               ApplyInverseNLTransformPixelList( NLTRANSFORMATION Transformation,
                                                            PIXELLIST List );
IMPORT(DWORD)               CreateInverseNLTransformedImage( IMG Image, NLTRANSFORMATION Transformation,
                                                            long TargetWidth, long TargetHeight, 
                                                            long XTargetOffset, long YTargetOffset,
                                                            IMG &Out );
IMPORT(DWORD)               CreateNLTransform             ( PIXELLIST OriginalPixels,
                                                            PIXELLIST TransformedPixels,
                                                            long Order, TFProgress Callback,
                                                            void *PrivateData, NLTRANSFORMATION &Transformation,
                                                            double &Quality );
IMPORT(DWORD)               CreateNLTransformedImage      ( IMG Image, NLTRANSFORMATION Transformation,
                                                            long TargetWidth, long TargetHeight, 
                                                            long XTargetOffset, long YTargetOffset,
                                                            IMG &Out );
IMPORT(BOOL)                IsNLTransform                 ( NLTRANSFORMATION Transformation );
IMPORT(DWORD)               NLTransformNumCoefficients    ( NLTRANSFORMATION Transformation );
IMPORT(DWORD)               NLTransformOrder              ( NLTRANSFORMATION Transformation );
IMPORT(BOOL)                NLTransformCoefficients       ( NLTRANSFORMATION Transformation,
                                                            double *CoeffsTransX, double *CoeffsTransY,
                                                            double *CoeffsInvTransX, double *CoeffsInvTransY );
IMPORT(BOOL)                LoadNLTransformFile           ( PCHAR FileName, NLTRANSFORMATION &Transformation );
IMPORT(BOOL)                WriteNLTransformFile          ( NLTRANSFORMATION Transformation, PCHAR FileName );

// Functions that convert the dynamic range of images
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               ConvertTo8BPPUnsigned         ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ConvertTo16BPPUnsigned        ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ConvertTo16BPPSigned          ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ConvertTo32BPPSigned          ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ConvertTo32BPPFloat           ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ScaleTo8BPPUnsigned           ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ScaleTo16BPPUnsigned          ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ScaleTo16BPPSigned            ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ScaleTo32BPPSigned            ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               ScaleTo32BPPFloat             ( IMG ImgIn, IMG &ImgOut, 
                                                            double MinVal, double MaxVal );

// Functions that manipulate Image contents
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               InitializeImageRect           ( IMG ImgIn, long left, long top,
                                                            long right, long bottom, double *ValuesIn );
IMPORT(DWORD)               SwapRGBChannels               ( IMG ImgIn, long Order[3], IMG &ImgOut );
IMPORT(DWORD)               AddUniformNoise               ( IMG ImgIn, double Lowest, double Highest,
                                                            long Seed, IMG &ImgOut );
IMPORT(DWORD)               AddGaussNoise                 ( IMG ImgIn, double Mean, double StdDev,
                                                            long Seed, IMG &ImgOut );
IMPORT(DWORD)               CreateFilterTestImage         ( long Width, long Height, long Dimension,
                                                            long BitsPerPixel, BOOL Signed, BOOL Float,
                                                            IMG &ImgOut );
IMPORT(DWORD)               CreateGreyRamp                ( long Width, long Height, long Dimension,
                                                            long BitsPerPixel, BOOL Signed, BOOL Float,
                                                            double Offset, double Slope, 
                                                            BOOL AxisHorizontal, BOOL AxisVertical,
                                                            IMG &ImgOut );

// Arithmetic and logical functions
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               AddImages                     ( IMG ImgIn1, IMG ImgIn2, 
                                                            BOOL ScaleResults, IMG &ImgOut );
IMPORT(DWORD)               AddConstant                   ( IMG ImgIn, double Values[], 
                                                            BOOL ScaleResults, IMG &ImgOut );
IMPORT(DWORD)               MultiplyImages                ( IMG ImgIn1, IMG ImgIn2,
                                                            BOOL ScaleResults, IMG &ImgOut );
IMPORT(DWORD)               MultiplyConstant              ( IMG ImgIn, double Values[], 
                                                            BOOL ScaleResults, IMG &ImgOut );
IMPORT(DWORD)               SubtractImages                ( IMG ImgIn1, IMG ImgIn2,
                                                            IMG &ImgOut );
IMPORT(DWORD)               SubtractConstant              ( IMG ImgIn, double Values[], 
                                                            IMG &ImgOut );
IMPORT(DWORD)               DivideImages                  ( IMG ImgIn1, IMG ImgIn2,
                                                            long UpShift,
                                                            IMG &ImgOut );
IMPORT(DWORD)               DivideConstant                ( IMG ImgIn, double Values[], 
                                                            IMG &ImgOut );
IMPORT(DWORD)               AbsoluteImage                 ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               SubtractImagesAbsolute        ( IMG ImgIn1, IMG ImgIn2,
                                                            IMG &ImgOut );
IMPORT(DWORD)               SubtractConstantAbsolute      ( IMG ImgIn, double Values[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               SquareImage                   ( IMG ImgIn, BOOL ScaleResults,
                                                            IMG &ImgOut );
IMPORT(DWORD)               SquareRootImage               ( IMG ImgIn, BOOL ScaleResults,
                                                            IMG &ImgOut );
IMPORT(DWORD)               AndImages                     ( IMG ImgIn1, IMG ImgIn2,
                                                            IMG &ImgOut );
IMPORT(DWORD)               AndConstant                   ( IMG ImgIn, long Values[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               NegateImage                   ( IMG ImgIn, IMG &ImgOut );
IMPORT(DWORD)               OrImages                      ( IMG ImgIn1, IMG ImgIn2,
                                                            IMG &ImgOut );
IMPORT(DWORD)               OrConstant                    ( IMG ImgIn, long Values[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               XorImages                     ( IMG ImgIn1, IMG ImgIn2,
                                                            IMG &ImgOut );
IMPORT(DWORD)               XorConstant                   ( IMG ImgIn, long Values[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               UpShift                       ( IMG ImgIn, long Values[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               DownShift                     ( IMG ImgIn, long Values[],
                                                            IMG &ImgOut );

// Colour Space Conversion functions
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               ConvertRGBToYUV               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertYUVToRGB               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToYCbCr             ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertYCbCrToRGB             ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToXYZ               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertXYZToRGB               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToLUV               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertLUVToRGB               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToLab               ( IMG ImgIn,
                                                            BOOL UseLab16,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertLabToRGB               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToYCC               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertYCCToRGB               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToHLS               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertHLSToRGB               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToHSV               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertHSVToRGB               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToGrayScaleStandard ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ConvertRGBToGrayScale         ( IMG ImgIn,
                                                            double CoeffRed, double CoeffGreen, double CoeffBlue,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ApplyColorTwistMatrix         ( IMG ImgIn,
                                                            double A11, double A12, double A13, double A14,
                                                            double A21, double A22, double A23, double A24,
                                                            double A31, double A32, double A33, double A34,
                                                            IMG &ImgOut );

// LUT functions
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               ApplyLUTGeneric               ( IMG ImgIn,
                                                            long Index,
                                                            long NumLevels,
                                                            double Levels[],
                                                            double Values[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               ApplyLUTLinear                ( IMG ImgIn,
                                                            long Index,
                                                            long NumLevels,
                                                            double Levels[],
                                                            double Values[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               ApplyLUTCubic                 ( IMG ImgIn,
                                                            long Index,
                                                            long NumLevels,
                                                            double Levels[],
                                                            double Values[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               ApplyLUT8Bit                  ( IMG ImgIn,
                                                            long Index,
                                                            long Values[256],
                                                            IMG &ImgOut );


// Thresholding Functions
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               CreateDynamicThresholdImage   ( IMG ImgIn, long WindowSize,
                                                            long Threshold, IMG &ImgOut );
IMPORT(DWORD)               CreateDynamicThresholdImageEx ( IMG ImgIn, TDynamicThresholdNorm Norm,
                                                            long WindowSize, double Threshold, 
                                                            IMG &ImgOut );
IMPORT(DWORD)               CreateImageThresholdImage     ( IMG ImgIn, IMG ImgThreshold,
                                                            TThresholdComparison Comparison,
                                                            IMG &ImgOut );
IMPORT(DWORD)               CreateConstantThresholdImage  ( IMG ImgIn, double Values[],
                                                            TThresholdComparison Comparison,
                                                            IMG &ImgOut );

// Morphological Functions
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               ErodeImage                    ( IMG ImgIn,
                                                            TMorphologyMask MaskType,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            IMG CustomMask,
                                                            IMG &ImgOut );
IMPORT(DWORD)               DilateImage                   ( IMG ImgIn,
                                                            TMorphologyMask MaskType,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            IMG CustomMask,
                                                            IMG &ImgOut );
IMPORT(DWORD)               OpeningImage                  ( IMG ImgIn,
                                                            TMorphologyMask MaskType,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            IMG CustomMask,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ClosingImage                  ( IMG ImgIn,
                                                            TMorphologyMask MaskType,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            IMG CustomMask,
                                                            IMG &ImgOut );

// Filter Functions
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               FilterBoxMean                 ( IMG ImgIn,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterBoxMin                  ( IMG ImgIn,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterBoxMax                  ( IMG ImgIn,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterBoxMedian               ( IMG ImgIn,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterMedianColor             ( IMG ImgIn,
                                                            TFilterMask MaskType,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterUserLinear              ( IMG ImgIn,
                                                            long KernelWidth, long KernelHeight,
                                                            long KernelOffsetX, long KernelOffsetY,
                                                            double KernelValues[],
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterWiener                  ( IMG ImgIn,
                                                            long MaskWidth, long MaskHeight,
                                                            long MaskOffsetX, long MaskOffsetY,
                                                            double NoiseThreshold,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterPrewittHorizontal       ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterPrewittVertical         ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterScharrHorizontal        ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterScharrVertical          ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterSobelHorizontal         ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterSobelVertical           ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterSobelHorizontal2ndOrder ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterSobelVertical2ndOrder   ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterSobelCross              ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterRobertsDown             ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterRobertsUp               ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterLaplace                 ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterGauss                   ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterHiPass                  ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterLoPass                  ( IMG ImgIn,
                                                            TFilterMask MaskSize,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterSharpening              ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               FilterCanny                   ( IMG ImgIn, long Index,
                                                            TCannyEdgeFilter Filtering,
                                                            long ThresholdLower,
                                                            long ThresholdUpper,
                                                            IMG &ImgOut );

// Statistical Functions
// --------------------------------------------------------------------------------------
IMPORT(BOOL)                IsImageMoments                ( IMGMOMENTS Moments );
IMPORT(IMGMOMENTS)          CreateImageMoments            ( TMomentsCalculation CalcMode );
IMPORT(DWORD)               CalculateMoments              ( IMG ImgIn, long Index,
                                                            long left, long top,
                                                            long right, long bottom,
                                                            IMGMOMENTS Moments );
IMPORT(DWORD)               GetSpatialImageMoment         ( IMGMOMENTS Moments,
                                                            long XOrder, long YOrder,
                                                            long OffsetX, long OffsetY,
                                                            BOOL Normalized, double &Moment );
IMPORT(DWORD)               GetCentralImageMoment         ( IMGMOMENTS Moments,
                                                            long XOrder, long YOrder,
                                                            BOOL Normalized, double &Moment );
IMPORT(DWORD)               GetHuImageMoments             ( IMGMOMENTS Moments,
                                                            double &M1, double &M2,
                                                            double &M3, double &M4,
                                                            double &M5, double &M6,
                                                            double &M7 );
IMPORT(DWORD)               ImageNormLInf                 ( IMG ImgIn, long Index,
                                                            long left, long top,
                                                            long right, long bottom,
                                                            BOOL Normalize, double &Value );
IMPORT(DWORD)               ImageNormL1                   ( IMG ImgIn, long Index,
                                                            long left, long top,
                                                            long right, long bottom,
                                                            BOOL Normalize, double &Value );
IMPORT(DWORD)               ImageNormL2                   ( IMG ImgIn, long Index,
                                                            long left, long top,
                                                            long right, long bottom,
                                                            BOOL Normalize, double &Value );
IMPORT(DWORD)               WangImageQuality              ( IMG ImgIn1, long Index1,
                                                            IMG ImgIn2, long Index2,
                                                            double &Value );

// Correlation functions
// --------------------------------------------------------------------------------------
IMPORT(DWORD)               CalculateSumOfSquareDistances ( IMG ImgIn, long IndexIn,
                                                            IMG Template, long IndexTemplate,
                                                            long left, long top,
                                                            long right, long bottom,
                                                            IMG &Accumulator );
IMPORT(DWORD)               CalculateCrossCorrelations    ( IMG ImgIn, long IndexIn,
                                                            IMG Template, long IndexTemplate,
                                                            long left, long top,
                                                            long right, long bottom,
                                                            IMG &Accumulator );
IMPORT(DWORD)               CalculateCorrelationCoefficients( IMG ImgIn, long IndexIn,
                                                            IMG Template, long IndexTemplate,
                                                            long left, long top,
                                                            long right, long bottom,
                                                            IMG &Accumulator );

// Geometric image transformations
// ------------------------------------------------------------------------------------------------
IMPORT(DWORD)               RotateImage                   ( IMG ImgIn,
                                                            double Angle,
                                                            TInterpolationMode Interpolation,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ResizeImage                   ( IMG ImgIn,
                                                            long TargetWidth, long TargetHeight,
                                                            TInterpolationMode Interpolation,
                                                            IMG &ImgOut );
IMPORT(DWORD)               MirrorImage                   ( IMG ImgIn,
                                                            BOOL MirrorX, BOOL MirrorY,
                                                            IMG &ImgOut );
IMPORT(DWORD)               ShearImage                    ( IMG ImgIn,
                                                            double ShearX, double ShearY,
                                                            TInterpolationMode Interpolation,
                                                            IMG &ImgOut );
IMPORT(DWORD)               PerspectiveTransformImage     ( IMG ImgIn,
                                                            long DstWidth, long DstHeight,
                                                            long DstXOffset, long DstYOffset,
                                                            double C00, double C01, double C02,
                                                            double C10, double C11, double C12,
                                                            double C20, double C21, double C22,
                                                            TInterpolationMode Interpolation,
                                                            IMG &ImgOut );
IMPORT(DWORD)               CalcPerspectiveTransformation ( long dstLeft, long dstTop,
                                                            long dstRight, long dstBottom,
                                                            double srcX0, double srcY0,
                                                            double srcX1, double srcY1,
                                                            double srcX2, double srcY2,
                                                            double srcX3, double srcY3,
                                                            double &C00, double &C01, double &C02,
                                                            double &C10, double &C11, double &C12,
                                                            double &C20, double &C21, double &C22 );
IMPORT(DWORD)               MatrixTransformImage          ( IMG ImgIn,
                                                            TMatrix Matrix,
                                                            TInterpolationMode Interpolation,
                                                            IMG & ImgOut );
IMPORT(DWORD)               AreaSubImage                  ( IMG ImgIn,
                                                            TArea Area,
                                                            TInterpolationMode Interpolation,
                                                            IMG &ImgOut );

// Other image analysis and processing functions
// ------------------------------------------------------------------------------------------------
IMPORT(DWORD)               LocalEigenValuesAndVectors    ( IMG ImgIn, long Index,
                                                            TFilterMask DerivatorSize,
                                                            TFilterMask BlurSize,
                                                            IMG &ImgLambda1,
                                                            IMG &ImgLambda2,
                                                            IMG &ImgEV1X,
                                                            IMG &ImgEV1Y,
                                                            IMG &ImgEV2X,
                                                            IMG &ImgEV2Y );
IMPORT(DWORD)               LocalMinEigenValues           ( IMG ImgIn, long Index,
                                                            TFilterMask DerivatorSize,
                                                            TFilterMask BlurSize,
                                                            IMG &ImgLambdaMin );
IMPORT(DWORD)               DistanceTransformation        ( IMG ImgIn, long Index,
                                                            TFilterMask MaskSize,
                                                            TDistanceNorm Norm,
                                                            IMG &ImgOut );
IMPORT(DWORD)               PyramidDownSample             ( IMG ImgIn,
                                                            IMG &ImgOut );
IMPORT(DWORD)               PyramidUpSample               ( IMG ImgIn,
                                                            IMG &ImgOut );

// Foundation-Package Blob Tool
// ------------------------------------------------------------------------------------------------
IMPORT(FBLOB)               FBlobCreate                   ( IMG Image, long Index );
IMPORT(BOOL)                IsFBlob                       ( FBLOB Handle );
IMPORT(DWORD)               FBlobSetImage                 ( FBLOB Handle, IMG Image, long Index );
IMPORT(DWORD)               FBlobSetArea                  ( FBLOB Handle, long Index,
                                                            long left, long top, 
                                                            long right, long bottom );
IMPORT(DWORD)               FBlobSetSkipBinarization      ( FBLOB Handle, BOOL SkipIt );
IMPORT(DWORD)               FBlobGetSkipBinarization      ( FBLOB Handle, BOOL &SkipIt );
IMPORT(DWORD)               FBlobSetObjectFeatureRange    ( FBLOB Handle, 
                                                            long FeatureMin, long FeatureMax );
IMPORT(DWORD)               FBlobGetObjectFeatureRange    ( FBLOB Handle,
                                                            long &FeatureMin, long &FeatureMax );
IMPORT(DWORD)               FBlobSetObjectFeature         ( FBLOB Handle,
                                                            long Threshold, long Feature );
IMPORT(DWORD)               FBlobSetLimitArea             ( FBLOB Handle,
                                                            long AreaMin, long AreaMax );
IMPORT(DWORD)               FBlobGetLimitArea             ( FBLOB Handle,
                                                            long &AreaMin, long &AreaMax );
IMPORT(DWORD)               FBlobSetObjectTouchBorder     ( FBLOB Handle, long Mask );
IMPORT(DWORD)               FBlobGetObjectTouchBorder     ( FBLOB Handle, long &Mask );
IMPORT(DWORD)               FBlobSetLimitWidth            ( FBLOB Handle,
                                                            long Min, long Max );
IMPORT(DWORD)               FBlobGetLimitWidth            ( FBLOB Handle,
                                                            long &Min, long &Max );
IMPORT(DWORD)               FBlobSetLimitHeight           ( FBLOB Handle,
                                                            long Min, long Max );
IMPORT(DWORD)               FBlobGetLimitHeight           ( FBLOB Handle,
                                                            long &Min, long &Max );
IMPORT(DWORD)               FBlobBinariseImage            ( IMG Src, TDRect *Rect, long Index,
                                                            long Low, long High, IMG Dst );
IMPORT(DWORD)               FBlobExec                     ( FBLOB Handle );
IMPORT(DWORD)               FBlobGetExecTime              ( FBLOB Handle, double &Time );
IMPORT(DWORD)               FBlobGetNumBlobs              ( FBLOB Handle, long &Count );
IMPORT(DWORD)               FBlobGetSize                  ( FBLOB Handle, long BlobNum, long &Size );
IMPORT(DWORD)               FBlobGetCenterEx              ( FBLOB Handle, long BlobNum, 
                                                            double &CenterX, double &CenterY );
IMPORT(DWORD)               FBlobGetBlobSize              ( FBLOB Handle, long BlobNum, long &Size );
IMPORT(DWORD)               FBlobSetMaxExecTime           ( FBLOB Handle, long MaxTimeMS );
IMPORT(DWORD)               FBlobGetMaxExecTime           ( FBLOB Handle, long &MaxTimeMS );
IMPORT(DWORD)               FBlobGetMoments               ( FBLOB Handle, long BlobNum,
                                                            double &Min, double &Max, 
                                                            double &Ratio, double &Angle );
IMPORT(DWORD)               FBlobReset                    ( FBLOB Handle );
IMPORT(DWORD)               FBlobSetSortMode              ( FBLOB Handle, long SortMode,
                                                            long X, long Y, long LastEntry,
                                                            long Order );
IMPORT(DWORD)               FBlobGetCenter                ( FBLOB Handle, long BlobNum, 
                                                            long &CenterX, long &CenterY );
IMPORT(DWORD)               FBlobSetSortParameter         ( FBLOB Handle, long SortMode,
                                                            long X, long Y, long Order );
IMPORT(DWORD)               FBlobSort                     ( FBLOB Handle );
IMPORT(DWORD)               FBlobGetBoundingBox           ( FBLOB Handle, long BlobNum, 
                                                            long &x, long &y, long &dx, long &dy );
IMPORT(DWORD)               FBlobGetMinMomentPoints       ( FBLOB Handle, long BlobNum, 
                                                            long &x0, long &y0, long &x1, long &y1 );


// Foundation-Package Math Utilities
// ------------------------------------------------------------------------------------------------
IMPORT(DWORD)               AddAngles                     ( ANGLE Angle1, ANGLE Angle2, ANGLE &Sum );
IMPORT(DWORD)               AddVector2DToPixelList        ( VECTOR2D Vector, PIXELLIST List );
IMPORT(DWORD)               AddVectors2D                  ( VECTOR2D Vector1, VECTOR2D Vector2, VECTOR2D &Result );
IMPORT(DWORD)               CompareAngles                 ( ANGLE Angle1, ANGLE Angle2, long &Result );
IMPORT(ANGLE)               CreateAngle                   ();
IMPORT(ANGLE)               CreateAngleRadians            ( double Rad );
IMPORT(ANGLE)               CreateAngleDegrees            ( double Deg );
IMPORT(ANGLE)               CreateAngleArgument           ( double X, double Y );
IMPORT(CIRCLE2D)            CreateCircle2D                ();
IMPORT(CIRCLE2D)            CreateCircle2DByRegression    ( PIXELLIST RegressionList );
IMPORT(CIRCLE2D)            CreateCircle2DParametric      ( double CenterX, double CenterY, double Radius );
IMPORT(LINE2D)              CreateLine2DAnalytical        ( double A, double B, BOOL AxplusB );
IMPORT(LINE2D)              CreateLine2DByRegression      ( PIXELLIST RegressionList );
IMPORT(LINE2D)              CreateLine2DDistanceOrientation( double Distance, ANGLE Direction );
IMPORT(LINE2D)              CreateLine2DNormalDistance    ( VECTOR2D Normal, double Distance );
IMPORT(LINE2D)              CreateLine2DTwoPoints         ( VECTOR2D Point1, VECTOR2D Point2 );
IMPORT(LINE2D)              CreateLine2DVectorOrientation ( VECTOR2D Vector, ANGLE Direction );
IMPORT(DWORD)               CreateNormalizedVector2D      ( VECTOR2D Input, VECTOR2D &Output );
IMPORT(DWORD)               CreateNormalVector2D          ( VECTOR2D Input, VECTOR2D &Output );
IMPORT(LINE2D)              CreatePerpendicularLine2D     ( LINE2D Line, VECTOR2D Point );
IMPORT(VECTOR2D)            CreateVector2D                ();
IMPORT(VECTOR2D)            CreateVector2DCartesian       ( double X, double Y );
IMPORT(VECTOR2D)            CreateVector2DFromPOINT       ( POINT Vertex );
IMPORT(VECTOR2D)            CreateVector2DRadial          ( ANGLE Ang, double Radius );
IMPORT(DWORD)               DoLine2DClip                  ( LINE2D Line, TDRect ClippingRect, VECTOR2D &Clip1, VECTOR2D &Clip2 );
IMPORT(DWORD)               GetAngleDegrees               ( ANGLE Angle, double &Degrees );
IMPORT(DWORD)               GetAngleRadians               ( ANGLE Angle, double &Radians );
IMPORT(DWORD)               GetAngleCosine                ( ANGLE Angle, double &Cosine );
IMPORT(DWORD)               GetAngleSine                  ( ANGLE Angle, double &Sine );
IMPORT(DWORD)               GetAngleTangent               ( ANGLE Angle, double &Tangent );
IMPORT(DWORD)               GetCircle2DCenter             ( CIRCLE2D Circle, double &CenterX, double &CenterY );
IMPORT(DWORD)               GetCircle2DRadius             ( CIRCLE2D Circle, double &Radius );
IMPORT(DWORD)               GetLine2DAXBFormula           ( LINE2D Line, double &A, double &B );
IMPORT(DWORD)               GetLine2DAYBFormula           ( LINE2D Line, double &A, double &B );
IMPORT(DWORD)               GetLine2DInclination          ( LINE2D Line, ANGLE &Inclination );
IMPORT(DWORD)               GetLine2DIntersectionAngle    ( LINE2D Line1, LINE2D Line2, ANGLE &Intersection );
IMPORT(DWORD)               GetLine2DIntersectionPoint    ( LINE2D Line1, LINE2D Line2, VECTOR2D &Result );
IMPORT(DWORD)               GetLine2DOriginDistance       ( LINE2D Line, double &Distance );
IMPORT(DWORD)               GetLine2DNormalVector         ( LINE2D Line, VECTOR2D &NormalVector );
IMPORT(DWORD)               GetLine2DVectorOrientation    ( LINE2D Line, VECTOR2D &Vector, ANGLE &Angle );
IMPORT(DWORD)               GetManyLine2DIntersectionPoint( LINE2D *LineArray, long ArrayLength, 
                                                            double ConvergenceThreshold, long MaxIterations, 
                                                            VECTOR2D &Point );
IMPORT(DWORD)               GetPointDistance2D            ( VECTOR2D Point1, VECTOR2D Point2, double &Distance );
IMPORT(DWORD)               GetPointLine2DDistance        ( LINE2D Line, VECTOR2D Point, double &Distance );
IMPORT(DWORD)               GetVector2DCartesian          ( VECTOR2D Input, double &X, double &Y );
IMPORT(DWORD)               GetVector2DLength             ( VECTOR2D Input, double &Length );
IMPORT(DWORD)               GetVector2DRadial             ( VECTOR2D Input, ANGLE &Angle, double &Radius );
IMPORT(DWORD)               InnerProduct2D                ( VECTOR2D Vector1, VECTOR2D Vector2, double &Result );
IMPORT(BOOL)                IsAngle                       ( ANGLE Obj );
IMPORT(DWORD)               IsAntiParallelVector2D        ( VECTOR2D Vector1, VECTOR2D Vector2, BOOL &Result );
IMPORT(BOOL)                IsCircle2D                    ( CIRCLE2D Handle );
IMPORT(DWORD)               IsEqualCircle2D               ( CIRCLE2D Circle1, CIRCLE2D Circle2, BOOL &Result );
IMPORT(DWORD)               IsEqualLine2D                 ( LINE2D Line1, LINE2D Line2, BOOL &Result );
IMPORT(DWORD)               IsEqualVector2D               ( VECTOR2D Vector1, VECTOR2D Vector2, BOOL &Result );
IMPORT(BOOL)                IsLine2D                      ( LINE2D Handle );
IMPORT(DWORD)               IsLine2DParallel              ( LINE2D Line1, LINE2D Line2, BOOL &Result );
IMPORT(DWORD)               IsLine2DPerpendicular         ( LINE2D Line1, LINE2D Line2, BOOL &Result );
IMPORT(DWORD)               IsParallelVector2D            ( VECTOR2D Vector1, VECTOR2D Vector2, BOOL &Result );
IMPORT(DWORD)               IsPerpendicularVector2D       ( VECTOR2D Vector1, VECTOR2D Vector2, BOOL &Result );
IMPORT(DWORD)               IsPointInCircle2D             ( CIRCLE2D Circle, 
                                                            double PointX, double PointY, BOOL &Result );
IMPORT(DWORD)               IsPointOnCircle2DRadius       ( CIRCLE2D Circle, double PointX, double PointY, 
                                                            double Epsilon, BOOL &Result );
IMPORT(BOOL)                IsVector2D                    ( VECTOR2D Handle );
IMPORT(DWORD)               MultiplyAngle                 ( ANGLE Angle, double Constant );
IMPORT(DWORD)               MultiplyVector2D              ( VECTOR2D Vector, double Scalar, VECTOR2D &Result );
IMPORT(DWORD)               SetAngleArgument              ( ANGLE Angle, double X, double Y );
IMPORT(DWORD)               SetAngleDegrees               ( ANGLE Angle, double Degrees );
IMPORT(void)                SetAngleLUTGranularityDeg     ( double Granularity );
IMPORT(void)                SetAngleLUTGranularityRad     ( double Granularity );
IMPORT(DWORD)               SetAngleRadians               ( ANGLE Angle, double Radians );
IMPORT(DWORD)               SetLine2DAnalytical           ( LINE2D Line, double A, double B, BOOL AxplusB );
IMPORT(DWORD)               SetLine2DByRegression         ( LINE2D Line, PIXELLIST RegressionList );
IMPORT(DWORD)               SetLine2DDistanceOrientation  ( LINE2D Line, double Distance, ANGLE Direction );
IMPORT(DWORD)               SetLine2DInclination          ( LINE2D Line, ANGLE Inclination );
IMPORT(DWORD)               SetLine2DNormalVector         ( LINE2D Line, VECTOR2D NormalVector );
IMPORT(DWORD)               SetLine2DOriginDistance       ( LINE2D Line, double Distance );
IMPORT(DWORD)               SetLine2DTwoPoints            ( LINE2D Line, VECTOR2D Point1, VECTOR2D Point2 );
IMPORT(DWORD)               SetVector2DCartesian          ( VECTOR2D Input, double X, double Y );
IMPORT(DWORD)               SetVector2DRadial             ( VECTOR2D Input, ANGLE Angle, double Radius );
IMPORT(DWORD)               SubtractAngles                ( ANGLE Angle1, ANGLE Angle2, ANGLE &Diff );
IMPORT(DWORD)               SubtractVectors2D             ( VECTOR2D Vector1, VECTOR2D Vector2, VECTOR2D &Result );


#endif // INCLUDED_ICVFOUNDATION
