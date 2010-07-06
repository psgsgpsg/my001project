#ifndef SORTRESULT_H
#define SORTRESULT_H
#include <bitset>
class SortResult
{
public:
	typedef enum {	NoError	= 0,
					EdgeErr = 1,
					BlobErr = 2,
					ShortErr= 3,
					HueErr	= 4,
					SobelErr= 5,
					DoubleEdge = 6,
					PartShort = 7,
	}EErrorCode;

public:
	SortResult();
	~SortResult();
public:
	bool NewPeriod	();
	bool SetResult	(EErrorCode eErrCode, size_t posIndex);
	unsigned int GetErrorPosition	()	const;
	size_t		 GetErrorCount		()	const;
	unsigned int GetErrorCode		()	const;

private:
	std::bitset<32> m_errPos;
	unsigned int m_errCode;
};

inline SortResult::SortResult()
:	m_errCode(0)
{ }

inline SortResult::~SortResult()
{ }

inline bool SortResult::NewPeriod( )
{
	m_errPos.reset();
	m_errCode	= 0;
	return true;
}

inline bool SortResult::SetResult (EErrorCode eErrCode, size_t posIndex)
{
	bool success = false;
	if(posIndex >=0 && posIndex <= 3)
	{
		m_errPos.set(posIndex);
		m_errCode |= eErrCode << (posIndex * 4);
		success = true;
	}
	return success;
}

inline unsigned int SortResult::GetErrorPosition() const
{
	return m_errPos.to_ulong();
}

inline unsigned int SortResult::GetErrorCount() const
{
	return m_errPos.count();
}

inline unsigned int SortResult::GetErrorCode() const
{
	return m_errCode;
}

#endif // SORTRESULT_H