#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。

/////////////////////////////////////////////////////////////////////////////
// CCvdisplayctrl1 包装类

class CCvdisplayctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CCvdisplayctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x289A0006, 0x32DD, 0x11D3, { 0x98, 0xF3, 0x0, 0x60, 0x8, 0x2C, 0x35, 0xE6 } };
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
    LB_NONE = 0,
    LB_RUBBER = 1,
    LB_FRAME = 2,
    LB_AREAMODE = 3,
    LB_SETORIGIN = 4,
    LB_DRAWPOINT = 5,
    LB_DRAWFILL = 6,
    LB_DRAWLINE = 7,
    LB_DRAWRECT = 8
}LeftButtonMode;
enum
{
    RB_NONE = 0,
    RB_ZOOM = 1,
    RB_MENU = 2
}RightButtonMode;
enum
{
    MW_NONE = 0,
    MW_ZOOM = 1
}MouseWheelMode;
enum
{
    CS_NORMAL = 0,
    CS_CROSSHAIR = 1
}CoordStyle;
enum
{
    Flat = 0,
    ThreeDimensional = 1
}Appearance;


// 操作
public:

// _DCVdisplay

// Functions
//

	BOOL SaveImageByDialog()
	{
		BOOL result;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL SaveImage(LPCTSTR Filename)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Filename);
		return result;
	}
	BOOL CopyImageToClipboard()
	{
		BOOL result;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL GetSelectedArea(double * x0, double * y0, double * x1, double * y1, double * x2, double * y2)
	{
		BOOL result;
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, x0, y0, x1, y1, x2, y2);
		return result;
	}
	BOOL SetSelectedArea(double x0, double y0, double x1, double y1, double x2, double y2)
	{
		BOOL result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, x0, y0, x1, y1, x2, y2);
		return result;
	}
	BOOL RemoveLabel(long ID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID);
		return result;
	}
	BOOL RemoveAllLabels()
	{
		BOOL result;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL SubAreaSelected()
	{
		BOOL result;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL MakeOverlayImage()
	{
		BOOL result;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL HighLightLabel(long ID, BOOL Highlight)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID, Highlight);
		return result;
	}
	BOOL GetLabelPosition(long ID, long * x, long * y)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID, x, y);
		return result;
	}
	BOOL AddLabel(LPCTSTR Text, BOOL CanDrag, long Color, long ID, long x, long y)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Text, CanDrag, Color, ID, x, y);
		return result;
	}
	void Refresh()
	{
		InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL RemoveUserObject(long ID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID);
		return result;
	}
	BOOL RemoveAllUserObjects()
	{
		BOOL result;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL GetUserObjectPosition(long ID, long Index, long * x, long * y)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID, Index, x, y);
		return result;
	}
	BOOL ImageToClient(long ImageX, long ImageY, long * ClientX, long * ClientY)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ImageX, ImageY, ClientX, ClientY);
		return result;
	}
	BOOL ClientToImage(long ClientX, long ClientY, long * ImageX, long * ImageY)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ClientX, ClientY, ImageX, ImageY);
		return result;
	}
	BOOL AddUserObject(LPCTSTR Text, BOOL CanDrag, BOOL XorOnly, long NumVertices, long ID, long Vertices)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Text, CanDrag, XorOnly, NumVertices, ID, Vertices);
		return result;
	}
	BOOL RemoveOverlayObject(long ID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID);
		return result;
	}
	BOOL RemoveAllOverlayObjects()
	{
		BOOL result;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL GetOverlayObjectPosition(long ID, long Index, long * x, long * y)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID, Index, x, y);
		return result;
	}
	BOOL HighLightOverlayObject(long ID, BOOL Highlight)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID, Highlight);
		return result;
	}
	BOOL MoveOverlayObject(long ID, long x, long y, BOOL MoveAbsolute)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID, x, y, MoveAbsolute);
		return result;
	}
	BOOL IsOverlayObjectAvailable(LPCTSTR ObjectName, long * ObjectType, long * NumVertices, long * NumVerticesUsed)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ObjectName, ObjectType, NumVertices, NumVerticesUsed);
		return result;
	}
	BOOL AddOverlayObject(LPCTSTR ObjectName, LPCTSTR Text, BOOL CanDrag, BOOL XorOnly, long Color, long HighLightColor, BOOL Filled, long ID, long * Vertices, long * ObjectData)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ObjectName, Text, CanDrag, XorOnly, Color, HighLightColor, Filled, ID, Vertices, ObjectData);
		return result;
	}
	BOOL GetDisplayZoom(long * CenterX, long * CenterY, long * ZoomFactor)
	{
		BOOL result;
		static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, CenterX, CenterY, ZoomFactor);
		return result;
	}
	BOOL SetDisplayZoom(long CenterX, long CenterY, long ZoomFactor)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, CenterX, CenterY, ZoomFactor);
		return result;
	}
	void Lock()
	{
		InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Unlock()
	{
		InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL RemoveAllOverlays()
	{
		BOOL result;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL SaveWindowToBitmap(long hWnd, LPCTSTR Filename)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, hWnd, Filename);
		return result;
	}
	BOOL HasLabel(long ID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID);
		return result;
	}
	BOOL HasOverlayObject(long ID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID);
		return result;
	}
	BOOL HasUserObject(long ID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ID);
		return result;
	}
	BOOL AddOverlayObjectNET(LPCTSTR ObjectName, LPCTSTR Text, BOOL CanDrag, BOOL XorOnly, long Color, long HighLightColor, BOOL Filled, long ID, long PixelList, void * ObjectData)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_I4 VTS_UNKNOWN ;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, ObjectName, Text, CanDrag, XorOnly, Color, HighLightColor, Filled, ID, PixelList, ObjectData);
		return result;
	}
	BOOL AddUserObjectNET(LPCTSTR Text, BOOL CanDrag, BOOL XorOnly, long ID, long PixelList)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Text, CanDrag, XorOnly, ID, PixelList);
		return result;
	}
	BOOL SetDisplayZoomEx(long CenterX, long CenterY, double ZoomFactor)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 ;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, CenterX, CenterY, ZoomFactor);
		return result;
	}
	BOOL GetDisplayZoomEx(long * CenterX, long * CenterY, double * ZoomFactor)
	{
		BOOL result;
		static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_PR8 ;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, CenterX, CenterY, ZoomFactor);
		return result;
	}
	double GetPanoramaZoomFactor()
	{
		double result;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
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
double GetDisplayGain()
{
	double result;
	GetProperty(0x2, VT_R8, (void*)&result);
	return result;
}
void SetDisplayGain(double propVal)
{
	SetProperty(0x2, VT_R8, propVal);
}
long GetDisplayOffset()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}
void SetDisplayOffset(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}
long GetLeftButtonMode()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}
void SetLeftButtonMode(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}
BOOL GetStatusImageSize()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}
void SetStatusImageSize(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}
BOOL GetStatusEmpty()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}
void SetStatusEmpty(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}
BOOL GetStatusScale()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}
void SetStatusScale(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}
BOOL GetStatusCurrentPos()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}
void SetStatusCurrentPos(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}
BOOL GetStatusGrayValue()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}
void SetStatusGrayValue(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}
BOOL GetScrollBars()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}
void SetScrollBars(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}
BOOL GetShowCoords()
{
	BOOL result;
	GetProperty(0xb, VT_BOOL, (void*)&result);
	return result;
}
void SetShowCoords(BOOL propVal)
{
	SetProperty(0xb, VT_BOOL, propVal);
}
BOOL GetDrawErase()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}
void SetDrawErase(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}
long GetGreenPage()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}
void SetGreenPage(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}
long GetRedPage()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}
void SetRedPage(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}
long GetBluePage()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}
void SetBluePage(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}
BOOL GetBevelInner()
{
	BOOL result;
	GetProperty(0x10, VT_BOOL, (void*)&result);
	return result;
}
void SetBevelInner(BOOL propVal)
{
	SetProperty(0x10, VT_BOOL, propVal);
}
BOOL GetBevelOuter()
{
	BOOL result;
	GetProperty(0x11, VT_BOOL, (void*)&result);
	return result;
}
void SetBevelOuter(BOOL propVal)
{
	SetProperty(0x11, VT_BOOL, propVal);
}
long GetCoordStyle()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}
void SetCoordStyle(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}
long GetAppearance()
{
	long result;
	GetProperty(DISPID_APPEARANCE, VT_I4, (void*)&result);
	return result;
}
void SetAppearance(long propVal)
{
	SetProperty(DISPID_APPEARANCE, VT_I4, propVal);
}
short GetBorderStyle()
{
	short result;
	GetProperty(DISPID_BORDERSTYLE, VT_I2, (void*)&result);
	return result;
}
void SetBorderStyle(short propVal)
{
	SetProperty(DISPID_BORDERSTYLE, VT_I2, propVal);
}
BOOL GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}
void SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}
OLE_HANDLE GethWnd()
{
	OLE_HANDLE result;
	GetProperty(DISPID_HWND, VT_I4, (void*)&result);
	return result;
}
void SethWnd(OLE_HANDLE propVal)
{
	SetProperty(DISPID_HWND, VT_I4, propVal);
}
CString GetStatusUserText()
{
	CString result;
	GetProperty(0x13, VT_BSTR, (void*)&result);
	return result;
}
void SetStatusUserText(CString propVal)
{
	SetProperty(0x13, VT_BSTR, propVal);
}
BOOL GetRulerEnabled()
{
	BOOL result;
	GetProperty(0x14, VT_BOOL, (void*)&result);
	return result;
}
void SetRulerEnabled(BOOL propVal)
{
	SetProperty(0x14, VT_BOOL, propVal);
}
BOOL GetDirectDrawEnabled()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}
void SetDirectDrawEnabled(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}
long GetAOOCount()
{
	long result;
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}
void SetAOOCount(long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}
CString GetAOOName()
{
	CString result;
	GetProperty(0x17, VT_BSTR, (void*)&result);
	return result;
}
void SetAOOName(CString propVal)
{
	SetProperty(0x17, VT_BSTR, propVal);
}
long GetAOOType()
{
	long result;
	GetProperty(0x18, VT_I4, (void*)&result);
	return result;
}
void SetAOOType(long propVal)
{
	SetProperty(0x18, VT_I4, propVal);
}
long GetAOONumVertices()
{
	long result;
	GetProperty(0x19, VT_I4, (void*)&result);
	return result;
}
void SetAOONumVertices(long propVal)
{
	SetProperty(0x19, VT_I4, propVal);
}
long GetAOONumVerticesUsed()
{
	long result;
	GetProperty(0x1a, VT_I4, (void*)&result);
	return result;
}
void SetAOONumVerticesUsed(long propVal)
{
	SetProperty(0x1a, VT_I4, propVal);
}
long GetAOOIndex()
{
	long result;
	GetProperty(0x1b, VT_I4, (void*)&result);
	return result;
}
void SetAOOIndex(long propVal)
{
	SetProperty(0x1b, VT_I4, propVal);
}
long GetRightButtonMode()
{
	long result;
	GetProperty(0x1c, VT_I4, (void*)&result);
	return result;
}
void SetRightButtonMode(long propVal)
{
	SetProperty(0x1c, VT_I4, propVal);
}
BOOL GetManualDisplayRefresh()
{
	BOOL result;
	GetProperty(0x41, VT_BOOL, (void*)&result);
	return result;
}
void SetManualDisplayRefresh(BOOL propVal)
{
	SetProperty(0x41, VT_BOOL, propVal);
}
BOOL GetOverlayDragAndDelete()
{
	BOOL result;
	GetProperty(0x42, VT_BOOL, (void*)&result);
	return result;
}
void SetOverlayDragAndDelete(BOOL propVal)
{
	SetProperty(0x42, VT_BOOL, propVal);
}
BOOL GetSyncRefreshToMonitor()
{
	BOOL result;
	GetProperty(0x43, VT_BOOL, (void*)&result);
	return result;
}
void SetSyncRefreshToMonitor(BOOL propVal)
{
	SetProperty(0x43, VT_BOOL, propVal);
}
long GetMouseWheelMode()
{
	long result;
	GetProperty(0x44, VT_I4, (void*)&result);
	return result;
}
void SetMouseWheelMode(long propVal)
{
	SetProperty(0x44, VT_I4, propVal);
}
long GetDisplayRectPercentage()
{
	long result;
	GetProperty(0x45, VT_I4, (void*)&result);
	return result;
}
void SetDisplayRectPercentage(long propVal)
{
	SetProperty(0x45, VT_I4, propVal);
}
unsigned long GetBackColor()
{
	unsigned long result;
	GetProperty(DISPID_BACKCOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetBackColor(unsigned long propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_UI4, propVal);
}


};
