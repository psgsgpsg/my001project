#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。

/////////////////////////////////////////////////////////////////////////////
// CCvimagectrl1 包装类

class CCvimagectrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CCvimagectrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xA9632CBF, 0x48B9, 0x11D3, { 0x99, 0xE, 0x0, 0x60, 0x8, 0x2C, 0x35, 0xE6 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:
enum
{
    CVC_E_OK = 0,
    CVC_E_ERROR = 1,
    CVC_E_PARAMETER = 2,
    CVC_E_FILEIO = 3,
    CVC_E_TIMEOUT = 4,
    CVC_E_MEMORY = 5,
    CVC_E_INVALIDPLANE = 6,
    CVC_E_UNSUPPORTEDDATATYPE = 7,
    CVC_E_INVALIDCAMERAPORT = 8,
    CVC_E_BOARDSELECT = 9,
    CVC_E_INVALIDTRIGGERMODE = 10,
    CVC_E_PROPERTYREAD = 11,
    CVC_E_PROPERTYWRITE = 12,
    CVC_E_INVALIDPORT = 13,
    CVC_E_PORTREAD = 14,
    CVC_E_PORTWRITE = 15,
    CVC_E_NOIMAGE = 16,
    CVC_E_NOINTERFACE = 17,
    CVC_E_BUSY = 18,
    CVC_E_NOTSUPPORTED = 19,
    CVC_E_GRABABORTED = 20,
    CVC_E_NOPIXELLIST = 21,
    CVC_E_NOTENOUGHDATA = 22,
    CVC_E_NOTRANSFORMATION = 23,
    CVC_E_LINEAR_ONLY = 24,
    CVC_E_DIVISIONBYZERO = 25,
    CVC_E_INVALIDDIMENSION = 26,
    CVC_E_INVALIDCOLORMODEL = 27,
    CVC_E_INVALIDDATATYPE = 28,
    CVC_E_WRONGOBJECT = 29,
    CVC_E_NOTREADY = 30,
    CVC_E_NOANGLE = 31,
    CVC_E_NOVECTOR2D = 32,
    CVC_E_NOLINE2D = 33,
    CVC_E_VECTOR2D_ZERO_LENGTH = 34,
    CVC_E_VECTORS_IDENTICAL = 35,
    CVC_E_LINE2D_VERTICAL = 36,
    CVC_E_LINE2D_HORIZONTAL = 37,
    CVC_E_NOARGUMENT = 38,
    CVC_E_LINE2D_UNDEFINED = 39,
    CVC_E_NOINTERSECTION = 40,
    CVC_E_NOCLIPPING = 41,
    CVC_E_NOTENOUGHLINES = 42,
    CVC_E_OVERFLOW = 43,
    CVC_E_NOCIRCLE = 44,
    CVC_E_ACCESS = 45,
    CVC_E_NOTPRESENT = 46,
    CVC_E_UNSUPPORTEDFEATURE = 47
}ErrorCode;
enum
{
    G2STATUS_IMAGES_AQCUIRED = 0,
    G2STATUS_IMAGES_LOST = 1,
    G2STATUS_IMAGES_LOST_LOCKED = 2,
    G2STATUS_IMAGES_LOCKED = 3,
    G2STATUS_IMAGES_PENDIG = 4,
    G2STATUS_GRAB_ACTIVE = 5,
    G2STATUS_TIMESTAMP = 6
}Grab2InfoType;


// 操作
public:

// _DCVimage

// Functions
//

	BOOL SaveImage(LPCTSTR Filename)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Filename);
		return result;
	}
	BOOL CopyImageToClipboard()
	{
		BOOL result;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL PasteImageFromClipboard()
	{
		BOOL result;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL LoadImageByDialog()
	{
		BOOL result;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL SaveImageByDialog()
	{
		BOOL result;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL LoadImage(LPCTSTR Filename)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Filename);
		return result;
	}
	BOOL ScanPlane(long Plane, long Density, BOOL UseCS)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Plane, Density, UseCS);
		return result;
	}
	BOOL ScanImage(long Density, BOOL UseCS)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Density, UseCS);
		return result;
	}
	BOOL CreateEmptyImage(long Dimension, long Width, long Height, BOOL ForceOverlay)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Dimension, Width, Height, ForceOverlay);
		return result;
	}
	BOOL LoadImageByUserDialog(LPCTSTR DefaultPath, LPCTSTR Titel, LPCTSTR Filter)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, DefaultPath, Titel, Filter);
		return result;
	}
	BOOL SaveImageByUserDialog(LPCTSTR DefaultPath, LPCTSTR Titel, LPCTSTR Filter)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, DefaultPath, Titel, Filter);
		return result;
	}
	void Lock()
	{
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Unlock()
	{
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL Clear(long Plane, double Value)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Plane, Value);
		return result;
	}
	long DataType(long Plane)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Plane);
		return result;
	}
	BOOL Snap()
	{
		BOOL result;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL SaveLossyImage(LPCTSTR Filename, double Quality)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_R8 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Filename, Quality);
		return result;
	}
	BOOL GetGrab2Status(long Grab2InfoType, double * Grab2Info)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_PR8 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Grab2InfoType, Grab2Info);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

long GetImage()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}
void SetImage(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}
long GetImageWidth()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}
void SetImageWidth(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}
long GetImageHeight()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}
void SetImageHeight(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}
long GetImageDimension()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}
void SetImageDimension(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}
CString GetFilename()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}
void SetFilename(CString propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}
BOOL GetIsGrabber()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetIsGrabber(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
long GetSerialNumber()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}
void SetSerialNumber(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}
double GetOriginX()
{
	double result;
	GetProperty(0x8, VT_R8, (void*)&result);
	return result;
}
void SetOriginX(double propVal)
{
	SetProperty(0x8, VT_R8, propVal);
}
double GetOriginY()
{
	double result;
	GetProperty(0x9, VT_R8, (void*)&result);
	return result;
}
void SetOriginY(double propVal)
{
	SetProperty(0x9, VT_R8, propVal);
}
BOOL GetEntire()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}
void SetEntire(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}
double GetScaleX()
{
	double result;
	GetProperty(0xb, VT_R8, (void*)&result);
	return result;
}
void SetScaleX(double propVal)
{
	SetProperty(0xb, VT_R8, propVal);
}
double GetScaleY()
{
	double result;
	GetProperty(0xc, VT_R8, (void*)&result);
	return result;
}
void SetScaleY(double propVal)
{
	SetProperty(0xc, VT_R8, propVal);
}
double GetAreaX0()
{
	double result;
	GetProperty(0xd, VT_R8, (void*)&result);
	return result;
}
void SetAreaX0(double propVal)
{
	SetProperty(0xd, VT_R8, propVal);
}
double GetAreaX1()
{
	double result;
	GetProperty(0xe, VT_R8, (void*)&result);
	return result;
}
void SetAreaX1(double propVal)
{
	SetProperty(0xe, VT_R8, propVal);
}
double GetAreaX2()
{
	double result;
	GetProperty(0xf, VT_R8, (void*)&result);
	return result;
}
void SetAreaX2(double propVal)
{
	SetProperty(0xf, VT_R8, propVal);
}
double GetAreaY0()
{
	double result;
	GetProperty(0x10, VT_R8, (void*)&result);
	return result;
}
void SetAreaY0(double propVal)
{
	SetProperty(0x10, VT_R8, propVal);
}
double GetAreaY1()
{
	double result;
	GetProperty(0x11, VT_R8, (void*)&result);
	return result;
}
void SetAreaY1(double propVal)
{
	SetProperty(0x11, VT_R8, propVal);
}
double GetAreaY2()
{
	double result;
	GetProperty(0x12, VT_R8, (void*)&result);
	return result;
}
void SetAreaY2(double propVal)
{
	SetProperty(0x12, VT_R8, propVal);
}
BOOL GetResetCS()
{
	BOOL result;
	GetProperty(0x13, VT_BOOL, (void*)&result);
	return result;
}
void SetResetCS(BOOL propVal)
{
	SetProperty(0x13, VT_BOOL, propVal);
}
double GetCSA11()
{
	double result;
	GetProperty(0x14, VT_R8, (void*)&result);
	return result;
}
void SetCSA11(double propVal)
{
	SetProperty(0x14, VT_R8, propVal);
}
double GetCSA12()
{
	double result;
	GetProperty(0x15, VT_R8, (void*)&result);
	return result;
}
void SetCSA12(double propVal)
{
	SetProperty(0x15, VT_R8, propVal);
}
double GetCSA21()
{
	double result;
	GetProperty(0x16, VT_R8, (void*)&result);
	return result;
}
void SetCSA21(double propVal)
{
	SetProperty(0x16, VT_R8, propVal);
}
double GetCSA22()
{
	double result;
	GetProperty(0x17, VT_R8, (void*)&result);
	return result;
}
void SetCSA22(double propVal)
{
	SetProperty(0x17, VT_R8, propVal);
}
double GetRotationX()
{
	double result;
	GetProperty(0x18, VT_R8, (void*)&result);
	return result;
}
void SetRotationX(double propVal)
{
	SetProperty(0x18, VT_R8, propVal);
}
double GetRotationY()
{
	double result;
	GetProperty(0x19, VT_R8, (void*)&result);
	return result;
}
void SetRotationY(double propVal)
{
	SetProperty(0x19, VT_R8, propVal);
}
BOOL GetGrab()
{
	BOOL result;
	GetProperty(0x1a, VT_BOOL, (void*)&result);
	return result;
}
void SetGrab(BOOL propVal)
{
	SetProperty(0x1a, VT_BOOL, propVal);
}
BOOL GetPingPongEnabled()
{
	BOOL result;
	GetProperty(0x1b, VT_BOOL, (void*)&result);
	return result;
}
void SetPingPongEnabled(BOOL propVal)
{
	SetProperty(0x1b, VT_BOOL, propVal);
}


};
