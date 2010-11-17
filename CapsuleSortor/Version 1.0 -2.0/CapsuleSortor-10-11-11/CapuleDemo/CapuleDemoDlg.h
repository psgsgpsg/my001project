// CapuleDemoDlg.h : 头文件
//

#pragma once
#include "iCVCImg.h"
#include "wtypes.h "

#include "svsgigecard.h"
#include "tmfdarchive.h"
#include "TImgBuffer.h"
#include "timagetmpl.h"
#include "ArcTemple.h "
#include <string>
#include <vector>
#include "cvdisplayctrl1.h"
#include "cvimagectrl1.h"
using namespace std;

#pragma comment(lib, "cvcimg.lib")
// CCapuleDemoDlg 对话框
class CCapuleDemoDlg : public CDialog
{
// 构造
public:
	CCapuleDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CAPULEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	TMfdArchive m_mfdFile;

	TImgBuffer m_buff;
	TImage<PelGray8> m_binaryImage;
	TImage<PelGray8>	m_enhanceImage;

	TImage<PelGray8>	m_sourceImage;
	vector<POINT>		m_tempSet;
	IMG					m_dispIM;

	std::string&	GetExeFileDir	();
	IMG				GetIMGFromBuffer(TImgBuffer & buffer);
	IMG				GetIMGFromImage(TImage<PelGray8> & image);
	void			GetTopPos(	TImage<unsigned char>		&image,
								TRect2D<int>				&rect, 
								TPoint2D<int>				&centre,
								ArcTemple					&temp);
	void GetTopPos(		TImage<unsigned char>		&image,
						TRect2D<int>					&rect, 
						TPoint2D<int>				&centre,
						vector<POINT>					&temp);
	TImage<PelGray8>	GetTImageFromIMG(IMG img);
	
public:
	CCvdisplayctrl1 m_display;
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButton5();
public:
	CCvimagectrl1 m_imageControl;
public:
	afx_msg void OnBnClickedButton6();
};
