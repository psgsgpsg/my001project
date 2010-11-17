// TSeriesComm.h: interface for the TSeriesComm class.

#ifndef TSERIESCOMM_H
#define TSERIESCOMM_H

#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

class TSeriesComm
{
public:
	typedef enum { eTxAbort, eRxAbort, eTxClear, eRxClear } EClearFlag; 
	TSeriesComm();
	~TSeriesComm();

public:
	bool Open	(size_t portIndex = 1);
	void Close	();
	bool Config	(size_t baudRate	=9600,
				 size_t parity		=0,
				 size_t dataBits	=8,
				 size_t stopBits	=0); 
	size_t Read	(void *buff,	size_t buffSize);
	size_t Write(void *buff,	size_t buffSize);

	bool   ClearBuffer( EClearFlag eFlag );
	bool   FlushBuffer( );

private:
	size_t		m_portIndex;
	bool		m_connected;
	HANDLE		m_hComm;
	OVERLAPPED	m_osRead,m_osWrite;
	COMSTAT		m_comStat;
};

#endif // TSERIESCOMM_H