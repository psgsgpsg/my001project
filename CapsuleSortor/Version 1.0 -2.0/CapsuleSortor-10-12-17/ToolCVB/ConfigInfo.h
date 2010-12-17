#ifndef CONFIGINFO_H
#define CONFIGINFO_H

#include "TypeDefine.h"
#include "TIniFile.h"

class ConfigInfo
{
public:
	~ConfigInfo();
	static ConfigInfo& TheConfigInfo();

	enum	PROTYPE
	{FIRST, SECOND};
	
public:
	bool LoadIniFile(const TString& fileName,	TIniFile::EnumPath ePath);
	bool SaveIniFile(const TString& fileName,	TIniFile::EnumPath ePath);

	//const CapsuleParam&	CapsuleInfo		()	const;
	const CapsuleParam& FirstCapsuleInfo()	const;
	const CapsuleParam&	SecondCapsuleInfo() const;
	const SortorParam&	ColorInfo		()	const;
	const SortorParam&	MonoInfo		()	const;
	const CameraParam&	SecondCamInfo	()	const;
	const CameraParam&	FirstCamInfo	()	const;
	const ValveParam&	ValveInfo		()	const;
	const ComCtrlParam& ComCtrlInfo		()  const;
	const RemainParam&	RemainInfo		(	PROTYPE type)	const;
	const RadiumRange&	RadiumRangeInfo	()	const;
	const MinboxWidthRange&	MinboxWidthInfo	() const;

	bool				Simulation		()	const;
	int					WorkMode		()	const;
	const int			Solution_X		()	const;
	const int			Solution_Y		()	const;
	const int			PKGCapability	()	const;


	//void CapsuleInfo	(const CapsuleParam&	param);
	void FirstCapsuleInfo	(	const CapsuleParam&		param); 
	void SecondCapsuleInfo	(	const CapsuleParam&		param);
	void ColorInfo			(	const SortorParam&		param);
	void MonoInfo			(	const SortorParam&		param);
	void SecondCamInfo		(	const CameraParam&		param);
	void FirstCamInfo		(	const CameraParam&		param);
	void ValveInfo			(	const ValveParam&		param);
	void ComCtrlInfo		(	const ComCtrlParam&		param);
	void RemainInfo			(	PROTYPE type, const RemainParam&		param);

private:
	//CapsuleParam	m_capsuleParam;
	CapsuleParam	m_firstCapsuleParam;
	CapsuleParam	m_secondCapsuleParam;

	ValveParam		m_valveParam;

	SortorParam		m_firstParam;
	SortorParam		m_secondParam;

	CameraParam		m_secondCamParam;
	CameraParam		m_firstCamParam;

	ComCtrlParam	m_comCtrl;
	int				m_simlation;
	int				m_workMode;
	int				m_solution_X;
	int				m_solution_Y;
	int				m_packageCapablity;

	RemainParam		m_colorRemain;
	RemainParam		m_monoRemain;

	RadiumRange		m_radiumRange;	

	MinboxWidthRange	m_minboxWidthRange;
private:
	ConfigInfo();
	ConfigInfo(const ConfigInfo& src);
	ConfigInfo& operator=(const ConfigInfo& src);
};

#endif //CONFIGINFO_H