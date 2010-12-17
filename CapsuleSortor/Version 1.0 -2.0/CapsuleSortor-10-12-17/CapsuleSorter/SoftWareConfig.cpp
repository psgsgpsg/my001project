// SoftWareConfig.cpp : implementation file
//
#include <algorithm>
#include <numeric>

#include "stdafx.h"
#include "capsulesorter.h"
#include "SoftWareConfig.h"
#include "capsulesorterdlg.h"
#include "TImageCard.h"
#include "TMfdArchive.h"
#include "TWinUtility.h"
#include "KeyBoardDlg.h"
#include "VertifyDlg.h"
#include "SpecifyCapsualeDlg.h"
#include "Translation.h"
#include "ToInteger.h"
#include "configinfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoftWareConfig dialog

CSoftWareConfig::CSoftWareConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CSoftWareConfig::IDD, pParent),
	m_IMGForPlayer(0),
	m_pProcessor(0)
{
	//{{AFX_DATA_INIT(CSoftWareConfig)
	m_colorBright = 0;
	m_monoBright = 0;
	m_profileEnable = FALSE;
	m_radius = 0;
	m_minboxWidth = 0;
	//}}AFX_DATA_INIT
}


void CSoftWareConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoftWareConfig)
	DDX_Control(pDX, IDC_SCRL_MINBOXWIDTH, m_minboxScroll);
	DDX_Control(pDX, IDC_SCL_RADIUS, m_radiusScroll);
	DDX_Control(pDX, IDC_TEMP_LIST, m_paramList);
	DDX_Control(pDX, IDC_CVDISPLAY, m_cvDisplay);
	DDX_Text(pDX, IDC_ColorMaxVal, m_colorBright);
	DDV_MinMaxUInt(pDX, m_colorBright, 0, 255);
	DDX_Text(pDX, IDC_MonoMaxVal, m_monoBright);
	DDV_MinMaxUInt(pDX, m_monoBright, 0, 255);
	DDX_Check(pDX, IDC_PROFILE, m_profileEnable);
	DDX_Text(pDX, IDC_RADIUS, m_radius);
	DDV_MinMaxInt(pDX, m_radius, 0, 999999);
	DDX_Text(pDX, IDC_EDT_MINBOXWIDTH, m_minboxWidth);
	DDV_MinMaxLong(pDX, m_minboxWidth, 0, 99999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoftWareConfig, CDialog)
	//{{AFX_MSG_MAP(CSoftWareConfig)
	ON_BN_CLICKED(IDC_LOAD_IMAGE, OnLoadImage)
	ON_BN_CLICKED(IDC_NEXT_FRAM, OnNextFrame)
	ON_BN_CLICKED(IDC_DROP_RESULT, OnDropResult)
	ON_BN_CLICKED(IDC_COMPLETE, OnComplete)
	ON_BN_CLICKED(IDC_CapSoftKeyBD, OnCapSoftKeyBD)
	ON_EN_CHANGE(IDC_Hue2Change, OnChangeHue2)
	ON_EN_CHANGE(IDC_Hue1Change, OnChangeHue1)
	ON_EN_CHANGE(IDC_SizeChange2, OnChangeSizeChange2)
	ON_EN_CHANGE(IDC_SizeChange1, OnChangeSizeChange1)
	ON_BN_CLICKED(IDC_PROFILE, OnProfile)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoftWareConfig message handlers

BOOL CSoftWareConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	memset(&m_curParam, 0, sizeof(m_curParam));

	//Title
	CString title;
	title.LoadString(IDS_TMPLCFG_TITLE);
	SetWindowText(title);

	//Init ListCtrl
	CRect 	rect;
	GetDlgItem(IDC_TEMP_LIST)->GetClientRect(&rect);
	InitInterface(rect);	

	//Init
//	CapsuleParam param	= CapsuleProc::CapsuleInfo();
//	SetListContent(0, param);
	
	CapsuleSorterDlg*	pMainDlg	= ((CapsuleSorterDlg*)AfxGetMainWnd());	
	CapsuleProc*		pMonoProc	= pMainDlg->GetProcessor(1);
	m_monoBright					= pMonoProc->RemainInfo().binaryLowThres;
	m_secondCapsuleParam			= pMonoProc->CapsuleInfo();
	CapsuleProc*		pColorProc	= pMainDlg->GetProcessor(3);
	m_colorBright					= pColorProc->RemainInfo().binaryLowThres;
	m_firstCapsuleParam				= pColorProc->CapsuleInfo();

	const RadiumRange	config		= ConfigInfo::TheConfigInfo().RadiumRangeInfo();
	const RemainParam	remainConfig = ConfigInfo::TheConfigInfo().RemainInfo(ConfigInfo::SECOND);

	m_radiusScroll.SetScrollRange(config.minRadium, config.maxRadium);
	m_radiusScroll.SetScrollPos(remainConfig.radius);
	m_radius = m_radiusScroll.GetScrollPos();

	const MinboxWidthRange	minboxRange = ConfigInfo::TheConfigInfo().MinboxWidthInfo();
	m_minboxScroll.SetScrollRange(minboxRange.minWidth, minboxRange.maxWidth);
	m_minboxScroll.SetScrollPos(remainConfig.minBoxWidth);
	m_minboxWidth = m_minboxScroll.GetScrollPos();

	//Position
	GetWindowRect(&rect);
	int	scWidth	= GetSystemMetrics(SM_CXSCREEN);
	int	scHeight= GetSystemMetrics(SM_CYSCREEN);
	int width	= rect.right  - rect.left;
	int height	= rect.bottom - rect.top;
	int left	= (scWidth - width)/2;
	int top		= 0;
	MoveWindow(left, top, width, height);

	ShowWindow(SW_SHOW);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoftWareConfig::OnLoadImage() 
{
	CString		fileName;
	CVertifyDlg dlg;
	dlg.SetInformation("加载'Template.mfd'文件？");
	if(IDOK == dlg.DoModal())
	{
		fileName = TWinUtility::GetExeFileDir().c_str();
		dlg.SetInformation("加载mfd文件？\n是:第二个相机 否:第一个相机");
		if(IDOK == dlg.DoModal())
		{
			fileName += "\\TemplateMono.mfd";
		}
		else
		{
			fileName += "\\TemplateColor.mfd";
		}

	}
	else
	{
		PostMessage(WM_COMMAND, IDC_SOFTKEYBD);
		CFileDialog dlg( TRUE,									//file open
						_T("mfd"),								//file ext
						_T("*.mfd"),							//file name
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,	// flags
						_T("MultiFrame file (*.mfd)|*.mfd|"));	// filter

		if(IDOK == dlg.DoModal())
		{
			fileName = dlg.GetPathName();
		}
	}
	if(!LoadMutiFrameImage(fileName))
	{
		MessageBox("加载文件失败！");
	}
}

bool	CSoftWareConfig::LoadMutiFrameImage(LPCSTR filePath)
{
	bool success = false;
	TMfdArchive mfdFile;
	if(mfdFile.Load(filePath))
	{
		m_imgBuff = mfdFile.GetImgBuffer();
		if(!m_imgBuff.IsNull())
		{
			TImgDim dim = m_imgBuff.Dimension();
			
			if(3==dim.bytesPerPixel)
			{	m_hue1Vect.clear(); m_hue2Vect.clear();	}
			else
			{	m_dimVect.clear();	}

			if(!(m_imgPlayBuff.Dimension() == dim))
			{
				m_imgPlayBuff.ReInit(dim);
				if(m_IMGForPlayer)	ReleaseImage(m_IMGForPlayer);

				long pitchPlane = (dim.bytesPerPixel == 1) ? 0 : 1;
			
				CreateImageFromPointer(	m_imgPlayBuff.FrameData(0), 
										m_imgPlayBuff.FrameBytes(),
										dim.width,
										dim.height, 
										dim.bytesPerPixel,
										8, 
										dim.bytesPerPixel, 
										dim.bytesPerRow,
										pitchPlane, 0, 0, 0, m_IMGForPlayer);
			}
			
			std::string fileName = filePath;
			if (fileName.find("Mono") <= fileName.size())
			{
				m_pProcessor = ((CapsuleSorterDlg*)AfxGetMainWnd())->GetProcessor(1);
			}
			else{
				m_pProcessor = ((CapsuleSorterDlg*)AfxGetMainWnd())->GetProcessor(3);
			}
			m_radiusScroll.SetScrollPos(m_pProcessor->RemainInfo().radius	);
			m_radius		= m_pProcessor->RemainInfo().radius;
			m_minboxWidth	= m_pProcessor->RemainInfo().minBoxWidth;

			
			m_imgBuff.GoNextFrame();
			DisplayImage();
			Process(m_imgBuff);
			success = true;
			UpdateData(FALSE);
		}
	}
	return success;
}

bool	CSoftWareConfig::DisplayImage()
{
	TImgDim imgDim = m_imgBuff.Dimension();
	m_imgPlayBuff.PushData	(	m_imgBuff.CurrentFrameData(), imgDim);
	m_cvDisplay.SetImage	(	(long)m_IMGForPlayer);
	m_cvDisplay.Refresh		();
	return true;
}

void CSoftWareConfig::OnNextFrame() 
{
	// TODO: Add your control notification handler code here
	if(m_imgBuff.IsNull()) return;

	CapsuleParam param;
	EIndex index= m_pProcessor->GetProcessIndex();

	switch(index)
	{
	case eFirst:
		param = GetListContent(0);
		break;
	case eSecond:
		param = GetListContent(2);
		break;
	}

	TImgDim	dim = m_imgBuff.Dimension();
	if((3 == dim.bytesPerPixel) && (!m_hue1VectPerFrame.empty()))
	{
		m_hue1Vect.insert(m_hue1Vect.end(), m_hue1VectPerFrame.begin(), m_hue1VectPerFrame.end());
		m_hue2Vect.insert(m_hue2Vect.end(), m_hue2VectPerFrame.begin(), m_hue2VectPerFrame.end());

		param.capColor	= GetStdHue(m_hue1Vect);
		param.bodyColor = GetStdHue(m_hue2Vect);
	}
	
	if((1 == dim.bytesPerPixel) && (!m_dimVectPerFrame.empty()))
	{
		m_dimVect.insert(m_dimVect.end(), m_dimVectPerFrame.begin(), m_dimVectPerFrame.end());
		param= GetStdDim(m_dimVect);
	}
	
	switch(index)
	{
	case eFirst:
		SetListContent(0, param);
		break;
	case eSecond:
		SetListContent(2, param);
		break;
	}

	m_imgBuff.GoNextFrame();
	DisplayImage();	
	Process(m_imgBuff);
}

bool CSoftWareConfig::Process(const TImgBuffer &imgBuff)
{
	PixelMem*	pImage		= imgBuff.CurrentFrameData();
	size_t		bytes		= imgBuff.FrameBytes();
	TImgDim		dim			= imgBuff.Dimension();

	m_pProcessor->SetRawImage(pImage, bytes);
	CapsuleProc::ObservingTarget(100);
	m_pProcessor->Process(CapsuleProc::TRAIN);
	if (m_profileEnable)
	{
		m_cvDisplay.SetImage(reinterpret_cast<long>(CapsuleProc::ObservedIMG()));
	}	
	else
	{
		m_cvDisplay.SetImage(reinterpret_cast<long>(m_IMGForPlayer));
	}
//	CapsuleProc::ObservingTarget(999);
	

	TCapsuleParam& proData = m_pProcessor->GetProcedureData();
	int arryCount = 1;			

	if (arryCount)
	{
		if (3 == dim.bytesPerPixel)
		{
			m_hue1VectPerFrame.clear();
			m_hue2VectPerFrame.clear();
			PelLab24 hue1;
			PelLab24 hue2;
			PelLab24 color;
			for(int i = 0; i < arryCount; ++i)
			{	
				bool noEmptyHue1  = !m_hue1Vect.empty();
				bool noEmptyHue1F = !m_hue1VectPerFrame.empty();

				if( noEmptyHue1 || noEmptyHue1F)
				{
					if		(noEmptyHue1)	{	color = GetStdHue(m_hue1Vect);			}
					else if	(noEmptyHue1F)	{	color = GetStdHue(m_hue1VectPerFrame);	}
				
					size_t distHue1  =	(hue1.a - color.a)*(hue1.a - color.a)
									  + (hue1.b - color.b)*(hue1.b - color.b);

					size_t distHue2 =	(hue2.a - color.a)*(hue2.a - color.a)
									  + (hue2.b - color.b)*(hue2.b - color.b);
					if(distHue1 > distHue2)
					{
						color	= hue1;
						hue1	= hue2;
						hue2	= color;
					}
				}
				m_hue1VectPerFrame.push_back(hue1);
				m_hue2VectPerFrame.push_back(hue2);
			}
			m_curParam.capColor = GetStdHue(m_hue1VectPerFrame);
			m_curParam.bodyColor= GetStdHue(m_hue2VectPerFrame);
			SetListContent(1, m_curParam);					
		}
		else
		{
			EIndex index = m_pProcessor->GetProcessIndex();
			m_dimVectPerFrame.clear();
			m_dimVectPerFrame.push_back(proData.GetParam());		
			m_curParam = GetStdDim(m_dimVectPerFrame);
			switch(index)
			{
			case eFirst:
				SetListContent(1, proData.GetParam());
				break;
			case eSecond:
				SetListContent(3, proData.GetParam());
				break;
			}				
		}
	}
	return true;
}

void CSoftWareConfig::OnDropResult() 
{
	if (!m_imgBuff.FrameCount())
	{
		return;
	}	
	m_imgBuff.GoNextFrame();
	DisplayImage();
	Process(m_imgBuff);
}

void CSoftWareConfig::OnComplete() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CVertifyDlg dlg;
	dlg.SetInformation("确认修改？");
	if(IDOK != dlg.DoModal())  return;

	CapsuleSorterDlg* pMainDlg	= ((CapsuleSorterDlg*)AfxGetMainWnd());	

	CapsuleParam firstParam = GetListContent(0);
	CapsuleParam secondParam = GetListContent(2);
//	CapsuleProc::CapsuleInfo(newParam);

	CapsuleProc	*pMonoProc	= pMainDlg->GetProcessor(1);
	RemainParam monoParam	= pMonoProc->RemainInfo();
	pMonoProc->CapsuleInfo(secondParam);
	monoParam.binaryLowThres		= m_monoBright;
	pMonoProc->RemainInfo(monoParam);
	//pMonoProc->CapsuleInfo(m_secondCapsuleParam);

	CapsuleProc	*pColorProc	= pMainDlg->GetProcessor(3);	
	RemainParam colorParam	= pColorProc->RemainInfo();
	pColorProc->CapsuleInfo(firstParam);
	colorParam.binaryLowThres = m_colorBright;
	pColorProc->RemainInfo(colorParam);
}

void CSoftWareConfig::SetListContent(size_t rowIndex, const CapsuleParam& param)
{
	CString txt;
	txt.Format("%d", param.capsuleDim.width);
	m_paramList.SetItemText(rowIndex, WIDTH, txt);
	txt.Format("%d", param.capsuleDim.height);
	m_paramList.SetItemText(rowIndex, HEIGHT, txt);
	txt.Format("%d", param.capsuleDim.tolerance);
	m_paramList.SetItemText(rowIndex, DIFF0, txt);
	
	txt.Format("%d", param.bodyLength);
	m_paramList.SetItemText(rowIndex, BODYLENGTH, txt);
	txt.Format("%d", param.capLength);
	m_paramList.SetItemText(rowIndex, CAPLENGTH, txt);
		
	txt.Format("%d", param.partTolerance);
	m_paramList.SetItemText(rowIndex, TOLERANCE, txt);
	//txt.Format("%d", param.bodyColor.L);
	//m_paramList.SetItemText(rowIndex, DIFF2, txt);		
}

CapsuleParam CSoftWareConfig::GetListContent(size_t rowIndex)
{
	CapsuleParam param;
	CString txt = m_paramList.GetItemText(rowIndex, WIDTH);
	param.capsuleDim.width = atoi(txt);
	txt = m_paramList.GetItemText(rowIndex, HEIGHT);
	param.capsuleDim.height= atoi(txt);
	txt = m_paramList.GetItemText(rowIndex, DIFF0);
	param.capsuleDim.tolerance = atoi(txt);	

 	txt = m_paramList.GetItemText(rowIndex, BODYLENGTH);
 	param.bodyLength	= atoi(txt);
 	txt = m_paramList.GetItemText(rowIndex, CAPLENGTH);
 	param.capLength		= atoi(txt);
 
 	txt = m_paramList.GetItemText(rowIndex, TOLERANCE);
	param.partTolerance = atoi(txt);
 //	pos = txt.Find(',');
// 	param.bodyColor.a = atoi(txt.Left(pos));
// 	param.bodyColor.b = atoi(txt.Mid(pos+1));
// 	txt = m_paramList.GetItemText(rowIndex, DIFF2);
// 	param.bodyColor.L = atoi(txt);
	return param;
}

PelLab24 CSoftWareConfig::GetStdHue(const std::vector<PelLab24>& hueVect)
{	
	bool success = !hueVect.empty();
	PelLab24 lab;
	if(success)
	{
		int size	  = hueVect.size();
		int sum_a = 0, sum_b = 0;
		for(int i=0; i<size; ++i)
		{
			sum_a += hueVect[i].a;
			sum_b += hueVect[i].b;
		}
		lab.a = RoundToInt(1.0*sum_a / size);
		lab.b = RoundToInt(1.0*sum_b / size);

		sum_a = sum_b = 0;
		for(int j=0; j<size; ++j)
		{
			sum_a += (hueVect[j].a - lab.a) * (hueVect[j].a - lab.a);
			sum_b += (hueVect[j].b - lab.b) * (hueVect[j].b - lab.b);
		}
		double sqrtRoot_a  = sqrt(1.0*sum_a/size);
		double sqrtRoot_b  = sqrt(1.0*sum_b/size);
		double aqrtRoot_ab = sqrt(sqrtRoot_a*sqrtRoot_a + sqrtRoot_b*sqrtRoot_b);
		lab.L = static_cast<char>( 1 + CeilToInt( 3.0*aqrtRoot_ab));
	}
	return lab;
}

CapsuleParam CSoftWareConfig::GetStdDim(const std::vector<CapsuleParam>& dimVect)
{
	bool success = !dimVect.empty();
	CapsuleParam dim;
	if(success)
	{
		int size	= dimVect.size();
		int sum_w	= 0;
		int	sum_h	= 0;

		for(int i=0; i<size; ++i)
		{
			sum_w += dimVect[i].capsuleDim.width;
			sum_h += dimVect[i].capsuleDim.height;
		}

		dim.capsuleDim.width  = RoundToInt(1.0*sum_w / size);
		dim.capsuleDim.height = RoundToInt(1.0*sum_h / size);
		
		sum_w = sum_h = 0;

		for(int j=0; j<size; ++j)
		{
			sum_w += (dimVect[j].capsuleDim.width  - dim.capsuleDim.width  ) * (dimVect[j].capsuleDim.width  - dim.capsuleDim.width);
			sum_h += (dimVect[j].capsuleDim.height - dim.capsuleDim.height) *  (dimVect[j].capsuleDim.height - dim.capsuleDim.height) ;
		}

		double sqrtRoot_w	= sqrt		(	1.0*sum_w/size);
		double sqrtRoot_h	= sqrt		(	1.0*sum_h/size);
		dim.capsuleDim.tolerance		= CeilToInt	(	2.0*sqrtRoot_h);

		int sum_body = 0;
		int sum_cap	 = 0;

		for(int i = 0; i < size; ++i)
		{
			sum_body += dimVect[i].bodyLength;
			sum_cap	 += dimVect[i].capLength;
		}

		dim.bodyLength  = RoundToInt(1.0*sum_body / size);
		dim.capLength	= RoundToInt(1.0*sum_cap / size);

		sum_body = sum_cap = 0;

		for(int  j=0; j<size; ++j)
		{
			sum_body += (dimVect[j].bodyLength  - dim.bodyLength  ) * (dimVect[j].bodyLength  - dim.bodyLength);
			sum_cap += (dimVect[j].capLength - dim.capLength) *  (dimVect[j].capLength - dim.capLength) ;
		}

		double sqrtRoot_body	= sqrt		(	1.0*sum_body/size);
		double sqrtRoot_cap 	= sqrt		(	1.0*sum_cap/size);

		double maxRoot		= sqrtRoot_body > sqrtRoot_cap?sqrtRoot_body:sqrtRoot_cap;		
		dim.partTolerance		= CeilToInt	(	2.0 * maxRoot);		
	}
	return dim;
}

void CSoftWareConfig::OnCapSoftKeyBD() 
{
	CKeyBoardDlg::DisplayWindow();
}

void CSoftWareConfig::OnChangeHue1() 
{
	CString strVal;
	GetDlgItemText(IDC_Hue1Change, strVal);
	if(0 == atoi(LPCTSTR(strVal))) strVal = "0";
	m_paramList.SetItemText(0, TOLERANCE, strVal);
}

void CSoftWareConfig::OnChangeHue2() 
{
	CString strVal;
	GetDlgItemText(IDC_Hue2Change, strVal);
	if(0 == atoi(LPCTSTR(strVal))) strVal = "0";
	m_paramList.SetItemText(2, TOLERANCE, strVal);
}

void CSoftWareConfig::InitInterface(CRect	&rect)
{	
	const size_t  listParamNum = 7;	
	size_t itemWidth = rect.Width() / listParamNum;
	char*	MonoTitle[] = {	_T("参   数"),
							_T("宽  度"),
							_T("高  度"),
							_T("容差"),
							_T("体长"),
							_T("帽长"),
							_T("容差")};
	
	m_paramList.SetExtendedStyle(LVS_EX_GRIDLINES );

	for (int i = 0; i < listParamNum; ++i)
	{
		m_paramList.InsertColumn(i,		MonoTitle[i], LVCFMT_LEFT, itemWidth);
	}	
	
	m_paramList.InsertItem(0, _T("一号合计"));
	m_paramList.InsertItem(1, _T("一号相机"));
	m_paramList.InsertItem(2, _T("二号合计"));
	m_paramList.InsertItem(3, _T("二号相机"));
}

void CSoftWareConfig::OnChangeSizeChange2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strVal;
	GetDlgItemText(IDC_SizeChange2, strVal);
	if(0 == atoi(LPCTSTR(strVal))) strVal = "0";
	m_paramList.SetItemText(2, DIFF0, strVal);
	
}

void CSoftWareConfig::OnChangeSizeChange1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString strVal;
	GetDlgItemText(IDC_SizeChange1, strVal);
	if(0 == atoi(LPCTSTR(strVal))) strVal = "0";
	m_paramList.SetItemText(0, DIFF0, strVal);
	
}

void CSoftWareConfig::OnProfile() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	Process(m_imgBuff);
}

void CSoftWareConfig::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	UpdateData(TRUE);
	if (!m_pProcessor)
	{
		return;
	}
	
	ASSERT(pScrollBar != NULL);
	int nCurPos = pScrollBar->GetScrollPos();
	switch (nSBCode)
	{
	case SB_LEFT: //    Scroll to far left.
		nCurPos = 1;
		break;
	case SB_LINELEFT: //    Scroll left.
		nCurPos--;
		break;
	case SB_LINERIGHT: //    Scroll right.
		nCurPos++;
		break;
	case SB_PAGELEFT: //    Scroll one page left.
		nCurPos -= 10;
		break;
	case SB_PAGERIGHT: //    Scroll one page right.
		nCurPos += 10;
		break;
	case SB_RIGHT: //    Scroll to far right.
		nCurPos = 100;
		break;
	case SB_THUMBPOSITION: //    Scroll to absolute position. The current position is specified by the nPos parameter.
	case SB_THUMBTRACK: //    Drag scroll box to specified position. The current position is specified by the nPos parameter.
		nCurPos = nPos;
	}

	int ID;
	ID = pScrollBar->GetDlgCtrlID();
	RemainParam remainConfig = m_pProcessor->RemainInfo();
	switch(ID)
	{
	case IDC_SCL_RADIUS:
		m_radius = nCurPos;		
		remainConfig.radius = m_radius;		
		break;
	case IDC_SCRL_MINBOXWIDTH:
		m_minboxWidth = nCurPos;
		remainConfig.minBoxWidth = m_minboxWidth;
		break;
	}

	pScrollBar->SetScrollPos(nCurPos);
	m_pProcessor->RemainInfo(remainConfig);
	Process(m_imgBuff);
	m_cvDisplay.Refresh();
	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
