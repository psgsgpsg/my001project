/****************************************************************************************
                     STEMMER IMAGING GmbH                       
-----------------------------------------------------------------------------------------

    Filename    : icvcimg.h

    Description : Standard header file for the
                  Common Vision Blox Image Manager
    
    Created by  : Martin Kersting

    Revision    : 1.1
    Revision    : 2.1  Port for C++ build (RS)
    Revision    : 2.2  Changed to si Datatypes (RS)
    Revision    : 2.3  Changed to standard Datatypes for easier maintenance. Sorted the
                       function list in different categories. (VG)
    Revision    : 2.4  Fixed a few return types from long to BOOL. (VG)

****************************************************************************************/


#ifndef CVCIMAGE_INCLUDE 
#define CVCIMAGE_INCLUDE


#include <stddef.h>


#ifdef __cplusplus
  #define IMPORT(t) extern "C" t __stdcall  
#else
  #define IMPORT(t) t __stdcall
#endif

 
// CVB object types
// --------------------------------------------------------------------------------------
typedef void *IMG;              // CVB image object (object supporting IImageVPA)
typedef void *OBJ;              // other CVB-IUnknown-derived objects
typedef void *PIXELLIST;        // Pixellist object
typedef void **PPVOID;          // alias for pointer to void*


// CVB license info
// --------------------------------------------------------------------------------------
#define LI_NOLICENSE         0  // no ImageManager (IM) license found
#define LI_COMMONVISIONBLOX  1  // IM licensed through CVB-type dongle
#define LI_CVC               2  // IM licensed through CVC-type dongle
#define LI_VISIONBLOX        3  // IM licensed through Visionblox dongle (obsolete!)
#define LI_MINOS             4  // IM licensed through Minos dongle (obsolete!)
#define LI_HARDWARE          5  // IM licensed through other hardware (e.g. nodelocked)


// Callback definitions commonly used in CVB
// --------------------------------------------------------------------------------------
// Image Scan Callbacks ScanPlaneUnary and ScanPlaneBinary
typedef long ( __stdcall *TFLine)        ( void * pPrivate, long LinesToDo );
typedef long ( __stdcall *TFPixelUnary)  ( void * pPrivate, void *pPixel, long x, long y );
typedef long ( __stdcall *TFPixelBinary) ( void * pPrivate, void *pPixel1, void *pPixel2, long x, long y );
// Callback used by CreateTransformedImage for coordinate transformation
typedef void ( __stdcall *TFNonlinear)   ( void * pPrivate, double& X, double& Y );
// Callback used by CreateNLTransform to indicate progress during creation of a nonlinear transformation
typedef BOOL ( __stdcall *TFProgress)    ( void * pPrivate, long StepsTotal, long StepsDone );
// Callback used by CreateImageFromPointer to indicate that the memory is no longer needed
typedef void ( __stdcall *PFFINALRELEASE)( void *pBufferBase, void *pUserData );


// Simple and complex type definitions commonly used in CVB
// --------------------------------------------------------------------------------------
// intpointer type (for backward compatibility with MSVC 6)
#if _MSC_VER < 1300
typedef long intptr_t;
#endif
// Histogram-related types
typedef char    TGray8Table [256]; 
typedef short   TGray16Table[256];  
typedef long    TGray32Table[256]; 
typedef double  TGray64Table[256];    
typedef long    THistogram  [256]; 
// 2D-transformation matrix
typedef struct  
{
  double A11;
  double A12;
  double A21;
  double A22;
} TMatrix;
// coordinate system as used by all CVB image objects
typedef struct  
{
  double OrgX;
  double OrgY;
  TMatrix Matrix;
} TCoordinateMap;
typedef TCoordinateMap *LPCOORDINATEMAP;
// rectangle structure with double-precision members
typedef struct  
{
  double Left;
  double Top;
  double Right;
  double Bottom;
} TDRect;
// general oriented area
typedef struct  
{
  double X0;
  double Y0;
  double X1;
  double Y1;
  double X2;
  double Y2;
} TArea;
// normalization modes recognized by CreateNormalizedImage
enum TNormalizeMode
{ 
  Normalize_MeanVariance    = 0,
  Normalize_MinMax          = 1
};
// fixed datatype descriptors
enum TDatatype
{
  DT_Signed   = 0x100,
  DT_Float    = 0x200,
  DT_Overlay  = 0x400
};
// colour model descriptors (as returned by ImageColorModel)
enum TColorModel
{
  CM_Guess_RGB  = -2,
  CM_Guess_Mono = -1,
  CM_Unknown    =  0,
  CM_Mono       =  1,
  CM_RGB        =  2,
  CM_YUV        =  3,
  CM_HSI        =  4,
  CM_YCbCr      =  5,
  CM_LUV        =  6,
  CM_Lab        =  7,
  CM_HLS        =  8,
  CM_YCC        =  9,
  CM_HSV        = 10,
  CM_XYZ        = 11
};
// a single VPA entry
typedef struct tagVPAEntry
{ 
  intptr_t XEntry;
  intptr_t YEntry;
} VPAEntry; 
// an array of VPA entries
typedef VPAEntry *PVPAT;


// CVB licensing-related functions
// --------------------------------------------------------------------------------------
IMPORT(long)                GetLicenseInfo                ();
IMPORT(long)                GetSerialNumber               ();


// Angle- and Matrix operations
// --------------------------------------------------------------------------------------
IMPORT(double)              Argument                      ( double ADX, double ADY );
IMPORT(bool)                InverseMatrix                 ( TMatrix A, TMatrix& AInv );
IMPORT(double)              MatrixDeterminant             ( TMatrix M );
IMPORT(void)                MatrixRTransform              ( double Xin, double Yin, TMatrix M, 
                                                            double& Xout, double& Yout );
IMPORT(void)                MatrixITransform              ( long Xin, long Yin, TMatrix M, 
                                                            long& Xout, long& Yout );
IMPORT(void)                MatrixTransformArea           ( TArea AreaIn, TMatrix Matrix, 
                                                            TArea& AreaOut );
IMPORT(void)                RotationMatrix                ( double Angle, TMatrix& M );
IMPORT(void)                SetMatrix                     ( double B11, double B12, 
                                                            double B21, double B22, 
                                                            TMatrix& M );


// Area and Coordinate system operations
// --------------------------------------------------------------------------------------
IMPORT(void)                BoundingRectangle             ( TArea Area, TCoordinateMap CS, 
                                                            TDRect& OutputRect );
IMPORT(long)                MaxImageArea                  ( IMG I, TArea& Area );
IMPORT(void)                SetArea                       ( double X0, double Y0, 
                                                            double X1, double Y1, 
                                                            double X2, double Y2, 
                                                            TArea& Area );
IMPORT(void)                SetRectArea                   ( double L, double T, 
                                                            double R, double B, 
                                                            TArea& Area );
IMPORT(void)                ComposeCoordinateMaps         ( TCoordinateMap CS1, TCoordinateMap CS2, 
                                                            TCoordinateMap& CSOut );
IMPORT(BOOL)                CoordinateMapIsTranslation    ( TCoordinateMap CS );
IMPORT(void)                CoordinateMapTransform        ( double XI, double YI, 
                                                            TCoordinateMap CS, 
                                                            double& XP, double& YP );
IMPORT(void)                CoordinateMapTransformArea    ( TArea AreaIn, TCoordinateMap CS, 
                                                            TArea& AreaOut );
IMPORT(void)                InitCoordinateMap             ( TCoordinateMap& CM );
IMPORT(BOOL)                InverseCoordinateMap          ( TCoordinateMap CSIn, 
                                                            TCoordinateMap& CSOut );



// Functions for Image- and general Object-Management
// --------------------------------------------------------------------------------------
IMPORT(BOOL)                IsImage                       ( OBJ P );
IMPORT(BOOL)                IsIUnknown                    ( OBJ P );
IMPORT(long)                RefCount                      ( OBJ P );
IMPORT(long)                ReleaseImage                  ( IMG I );
IMPORT(BOOL)                ReleaseObject                 ( OBJ& P );
IMPORT(long)                ReleaseIUnknown               ( OBJ P);
IMPORT(long)                ShareImage                    ( IMG I );
IMPORT(BOOL)                ShareObject                   ( OBJ P );



// Functions to retrieve and set image properties
// --------------------------------------------------------------------------------------
IMPORT(long)                BitsPerPixel                  ( long DataType );
IMPORT(long)                BytesPerPixel                 ( long DataType );
IMPORT(BOOL)                GetDatatypeMinMaxVal          ( long DataType, long &MinVal, long &MaxVal );
IMPORT(BOOL)                GetImageCoordinates           ( IMG I, TCoordinateMap& CS );
IMPORT(BOOL)                GetImageOrigin                ( IMG I, double& OrgX, double& OrgY );
IMPORT(BOOL)                HasOverlay                    ( IMG I );
IMPORT(long)                ImageColorModel               ( IMG ImgIn );
IMPORT(long)                ImageDatatype                 ( IMG I, long index );
IMPORT(long)                ImageDimension                ( IMG I );
IMPORT(long)                ImageHeight                   ( IMG I );
IMPORT(long)                ImageWidth                    ( IMG I );
IMPORT(BOOL)                IsAll8Bit                     ( IMG I );
IMPORT(BOOL)                IsFloatDatatype               ( long DataType );
IMPORT(BOOL)                IsGrabber                     ( OBJ P );
IMPORT(long)                IsOverlayImage                ( IMG I );
IMPORT(BOOL)                IsSignedDatatype              ( long DataType );
IMPORT(long)                NumPlanesNeededPerImage       ( TColorModel ColorModel );
IMPORT(BOOL)                SetImageCoordinates           ( IMG I, TCoordinateMap CS );
IMPORT(BOOL)                SetImageOrigin                ( IMG I, double OrgX, double OrgY );


// Coordinate transformation functions
// --------------------------------------------------------------------------------------
IMPORT(void)                ImageAreaToPixel              ( IMG I, TArea ImageArea, TArea& PixelArea );
IMPORT(void)                ImageToPixel                  ( IMG I, double XI, double YI, 
                                                            double& XP, double& YP );
IMPORT(BOOL)                ImageToPixelCoordinates       ( IMG I, double XI, double YI, 
                                                            double& XP, double& YP );
IMPORT(void)                PixelAreaToImage              ( IMG I, TArea PixelArea, TArea& ImageArea );
IMPORT(void)                PixelToImage                  ( IMG I, double XP, double YP, 
                                                            double& XI, double& YI );
IMPORT(BOOL)                PixelToImageCoordinates       ( IMG I, double XP, double YP, 
                                                            double& XI, double& YI );


// Functions that access or give access to image data
// --------------------------------------------------------------------------------------
IMPORT(long)                CopyImageArea                 ( IMG ImgSrc, IMG ImgDst, 
                                                            long SrcIndex, long DstIndex, 
                                                            TArea Area, long TargetX0, long TargetY0 );
IMPORT(BOOL)                CopyOverlay                   ( IMG ISource, IMG ITarget );
IMPORT(BOOL)                GetImageVPA                   ( IMG I, long Index, 
                                                            void **lpBase, PVPAT *VPAT );
IMPORT(long)                InitializeImageArea           ( IMG Img, long Index, 
                                                            TArea Area, long Value );
IMPORT(BOOL)                ImageHistogram                ( IMG I, long Index, 
                                                            long Density, TArea Area, 
                                                            THistogram& HGram );
IMPORT(long)                PixVal                        ( IMG I, long Index, 
                                                            long X, long Y );
namespace CVB
{ // due to a naming collision these two functions need to be accessed either through
  // 'CVB::GetPixel' and 'CVB::SetPixel' or with 'using namespace CVB;'
  IMPORT(BOOL)              GetPixel                      ( IMG I, long Index, 
                                                            long X, long Y, long& Value );
  IMPORT(BOOL)              SetPixel                      ( IMG I, long Index, 
                                                            long X, long Y, long& Value );
};
// All binary scan functions use the coordinate system of Image1 ( if UseCS is at all true ).
// For the ScanPLANE... functions 'PPixel' refers to a pointer to a pixel value in the specified color plane. 
IMPORT(long)                ScanPlaneUnary                ( IMG Image, long Index, 
                                                            TArea Area, long Density, 
                                                            BOOL UseCS, TFPixelUnary FPixel, 
                                                            TFLine FStartLine, TFLine FEndLine, 
                                                            void *PPrivate );
IMPORT(long)                ScanPlaneBinary               ( IMG Image1, IMG Image2, 
                                                            long Index1, long Index2, 
                                                            TArea Area, long Density, 
                                                            long DDX, long DDY, 
                                                            BOOL UseCS, TFPixelBinary FPixel, 
                                                            TFLine FStartLine, TFLine FEndLine, 
                                                            void *PPrivate );
// For the ScanIMAGE... functions 'PPixel' refers to a pointer to a an array of pointers, which
// refer to the values in consecutive color planes. The sequence of pointers is terminated with a Null pointer.
IMPORT(long)                ScanImageUnary                ( IMG Image, TArea Area, 
                                                            long Density, BOOL UseCS, TFPixelUnary FPixel,
                                                            TFLine FStartLine, TFLine FEndLine, 
                                                            void *PPrivate );
IMPORT(long)                ScanImageBinary               ( IMG Image1, IMG Image2, 
                                                            TArea Area, long Density, 
                                                            long DDX, long DDY, 
                                                            BOOL UseCS, TFPixelBinary FPixel, 
                                                            TFLine FStartLine, TFLine FEndLine, 
                                                            void *PPrivate );


// Functions that serialize or deserialize images
// (Note: Serialization will fail if no valid ImageManager license is present!)
// --------------------------------------------------------------------------------------
IMPORT(long)                ImageToMemorySize             ( IMG I );
IMPORT(BOOL)                ImageToMemory                 ( IMG I, void* Memory, long Capacity );
IMPORT(BOOL)                MemoryToImage                 ( void* Memory, long Capacity, IMG& Img );
IMPORT(BOOL)                LoadImageFile                 ( PCHAR FileName, IMG& Image );
#if defined WIN32
  IMPORT(BOOL)              CopyImageClipboard            ( IMG I, HWND HWindow );
  IMPORT(BOOL)              PasteClipboardImage           ( HWND HWindow, IMG& Image );
  IMPORT(BOOL)              WriteLossyImageFile           ( IMG I, double Quality, PCHAR FileName );
  IMPORT(BOOL)              WriteImageFile                ( IMG I, PCHAR FileName );
  #ifndef _CVIMAGEDLL_IGNOREDISPLAY_
    IMPORT(void)            ImageToDC                     ( IMG I, HDC DC, 
                                                            long Left, long Top, 
                                                            long SourceLeft, long SourceTop, 
                                                            long SourceRight, long SourceBottom, 
                                                            long TargetWidth, long TargetHeight,
                                                            long RedPage, long GreenPage, long BluePage, 
                                                            double Gain, long Offset );
    typedef void ( __stdcall *TImageToDCEx) (  IMG I, HDC DC, long Left, long Top, long SourceLeft, long SourceTop, long SourceRight, long SourceBottom, long TargetWidth, long TargetHeight, long RedPage, long GreenPage, long BluePage, double Gain,  long Offset );
  #endif
#endif


// Functions that create images either out of other images, or out of 'nowhere'
// --------------------------------------------------------------------------------------
IMPORT(BOOL)                CreateAreaSubImage            ( IMG ImgIn, TArea Area, IMG &ImgOut );
IMPORT(BOOL)                CreateColorTransform          ( IMG I, long TargetDimension, 
                                                            BOOL Reflect, void* Coefficients, 
                                                            IMG& Image );
IMPORT(BOOL)                CreateCompatibleImage         ( IMG I, long AWidth, long AHeight, 
                                                            BOOL ForceOverlay, IMG& Image );
IMPORT(BOOL)                CreateConcatenatedImage       ( IMG *ImgArray, long ArrayLength,
                                                            BOOL ShareMemory, IMG &ImgOut );
IMPORT(BOOL)                CreateDuplicateImage          ( IMG I, IMG& Image );
IMPORT(BOOL)                CreateGenericImage            ( long ADimension, long AWidth, long AHeight, 
                                                            BOOL ForceOverlay, IMG& Image );
IMPORT(BOOL)                CreateGenericImageDT          ( long ADimension, long AWidth, long AHeight, 
                                                            long Datatype, IMG& Image );
IMPORT(BOOL)                CreateImageDeleteList         ( IMG I, long Index, long Count, 
                                                            BOOL Share, IMG& Image );
IMPORT(long)                CreateImageFromPointer        ( void *pImageMem, size_t MemSize, 
                                                            long Width, long Height, 
                                                            long NumPlanes, long DataType, 
                                                            long PitchX, long PitchY, long PitchPlane, 
                                                            long PlaneOrder[], 
                                                            PFFINALRELEASE ReleaseCallback, void *pUserData, 
                                                            IMG &ImgOut );
IMPORT(BOOL)                CreateImageInsertList         ( IMG I1, IMG I2, long Index, 
                                                            BOOL Share, BOOL Intersect, IMG& Image );
IMPORT(BOOL)                CreateImageMap                ( IMG I, long SrcLeft, long SrcTop, 
                                                            long SrcRight, long SrcBottom, 
                                                            long DstWidth, long DstHeight, IMG& Image);
IMPORT(BOOL)                CreateImageSubList            ( IMG I, long Index, long Count, 
                                                            BOOL Share, IMG& Image );
IMPORT(BOOL)                CreateLinearCombinationImage  ( IMG I, BOOL Reflect, void* Coefficients, 
                                                            IMG& Image );
IMPORT(BOOL)                CreateMatrixTransformedImage  ( IMG Img, TMatrix Matrix, IMG& ImgOut );
IMPORT(BOOL)                CreateMatrixTransformedImageEx( IMG Img, TMatrix Matrix, IMG& ImgOut );
IMPORT(BOOL)                CreateMinMaxImage             ( IMG I, IMG& IMin, IMG& IMax );
IMPORT(BOOL)                CreateMeanVarianceImage       ( IMG I, IMG& IMean, IMG& IVariance );
IMPORT(BOOL)                CreateNormalizedImage         ( IMG I, TNormalizeMode Mode, 
                                                            double TargetMeanMin, double TargetVarianceMax,
                                                            IMG& Image );
IMPORT(BOOL)                CreateOverlayImage            ( IMG I, IMG& Image );
IMPORT(BOOL)                CreatePolarImage              ( IMG Img, double CX, double CY, 
                                                            double Alpha0, double RMin, double RMax, 
                                                            IMG& ImgOut );
IMPORT(BOOL)                CreatePolarImageEx            ( IMG Img, double CX, double CY, 
                                                            double Alpha0, double AlphaTotal,
                                                            double RMin, double RMax, 
                                                            IMG& ImgOut );
IMPORT(BOOL)                CreateSubImage                ( IMG Img, TArea Area, IMG& ImgOut );
IMPORT(BOOL)                CreateTransformedImage        ( IMG Image, long TargetWidth, 
                                                            long TargetHeight, TFNonlinear Transformation, 
                                                            void *pPrivate, IMG& ImgOut );
IMPORT(BOOL)                MapTo8Bit                     ( IMG I, BOOL CollectiveNormalization, 
                                                            IMG& ImgOut );


// Pixellist handling and operations
// --------------------------------------------------------------------------------------
IMPORT(BOOL)                AddPixel                      ( PIXELLIST List, double *Data );
IMPORT(PIXELLIST)           ConcatenatePixelLists         ( PIXELLIST ListA, PIXELLIST ListB );
IMPORT(BOOL)                CopyPixelList                 ( PIXELLIST Source, PIXELLIST &Target );
IMPORT(BOOL)                CreateOverlayFromPixelList    ( PIXELLIST List, IMG ImgIn, IMG& ImgOut );
IMPORT(PIXELLIST)           CreatePixelList               ( DWORD Dimension );
IMPORT(BOOL)                CreatePixelListFromOverlay    ( IMG ImgIn, long Dim,
                                                            long Left, long Top, 
                                                            long Right, long Bottom, 
                                                            PIXELLIST& List );
//#pragma deprecated(FindMaxima)    // use SeekLocalMaxima instead!
IMPORT(long)                FindMaxima                    ( IMG I, long PlaneIndex, 
                                                            long Left, long Top, 
                                                            long Right, long Bottom, 
                                                            long Locality, long Threshold, 
                                                            PIXELLIST& Maxima);
IMPORT(BOOL)                FindMaximaEx                  ( IMG I, long PlaneIndex, 
                                                            long Left, long Top, 
                                                            long Right, long Bottom, 
                                                            long Locality, double Threshold, 
                                                            PIXELLIST& Maxima);
IMPORT(BOOL)                IsPixels                      ( OBJ P );
IMPORT(BOOL)                ListPixel                     ( PIXELLIST Pixels, 
                                                            long ListIndex, 
                                                            long& X, long& Y, long& Z );
IMPORT(BOOL)                ListPixelEx                   ( PIXELLIST List, 
                                                            long Index, double *Data );
IMPORT(BOOL)                ModifyListPixel               ( PIXELLIST List, DWORD Index, double* Data );
IMPORT(long)                PixelListCount                ( PIXELLIST Pixels );
IMPORT(DWORD)               PixelListDimension            ( PIXELLIST List );
IMPORT(BOOL)                RemoveAllPixels               ( PIXELLIST List );
IMPORT(BOOL)                RemovePixel                   ( PIXELLIST List, DWORD Index );
IMPORT(BOOL)                SortPixelsByValue             ( PIXELLIST Pixels );
IMPORT(BOOL)                SortPixelsByPosition          ( PIXELLIST Pixels, long X, long Y );
IMPORT(BOOL)                SortPixelsEx                  ( PIXELLIST List, double *ProjectionVector );
IMPORT(BOOL)                TransformPixelListCoordinates ( PIXELLIST List, TCoordinateMap Coord );
IMPORT(BOOL)                TransformPixelListMatrix      ( PIXELLIST List, TMatrix A,
                                                            double OffsetX, double OffsetY );
IMPORT(BOOL)                TruncatePixelList             ( PIXELLIST Pixels, long N );


#endif // CVCIMAGE_INCLUDE 
