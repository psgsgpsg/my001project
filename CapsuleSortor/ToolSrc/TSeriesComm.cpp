// TSeriesComm.cpp: implementation of the TSeriesComm class.


#include "TSeriesComm.h"
#include <cstdio>
TSeriesComm::TSeriesComm()
: m_portIndex(-1),
  m_connected(false),
  m_hComm(INVALID_HANDLE_VALUE)
{
	memset(&m_osRead,0,sizeof(OVERLAPPED));
	m_osRead.hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);

	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osWrite.hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
}

TSeriesComm::~TSeriesComm()
{
	if(m_connected)			Close();
	if(m_osRead.hEvent)		CloseHandle(m_osRead.hEvent);
	if(m_osWrite.hEvent)	CloseHandle(m_osWrite.hEvent);
}

bool TSeriesComm::Open(size_t portIndex)
{
	if(true==m_connected)
	{
		if(m_portIndex==portIndex)	{ return true;	}
		else						{ Close();		}
	}
	m_portIndex = portIndex;
	char			port[8];
	COMMTIMEOUTS	TimeOuts;
	sprintf(port, "COM%u", m_portIndex);

	m_hComm = CreateFile(	port, 
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
							NULL); // 打开串口，异步操作

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		 return m_connected = true;
	}
	else
	{
		 m_connected = true;
	}

	TimeOuts.ReadIntervalTimeout			=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier		=10;
	TimeOuts.ReadTotalTimeoutConstant		=100;
	TimeOuts.WriteTotalTimeoutMultiplier	=10;
	TimeOuts.WriteTotalTimeoutConstant		=100;
	SetCommTimeouts(m_hComm, &TimeOuts);

	return m_connected;
}

void TSeriesComm::Close()
{
	if(m_connected)
	{
		CloseHandle(m_hComm);
		m_portIndex = -1;
		m_hComm		= INVALID_HANDLE_VALUE;
		m_connected	= false;
	}
}

bool TSeriesComm::Config(size_t baudRate,
						 size_t parity,
						 size_t dataBits,
						 size_t stopBits)
{
	DCB dcb;
	if(!m_connected || !GetCommState(m_hComm, &dcb))
	{	return false;	}

	dcb.fBinary		=TRUE;
	dcb.fParity		=TRUE;
	dcb.BaudRate	=baudRate;
	dcb.ByteSize	=dataBits;
	switch(parity)
	{
	case 0:
		dcb.Parity = NOPARITY;
		break;
	case 1:
		dcb.Parity = ODDPARITY;
		break;
	case 2:
		dcb.Parity = EVENPARITY;
		break;
	default:
		dcb.Parity = dcb.Parity;
	}

	switch(stopBits)
	{
	case 0:
		dcb.StopBits = ONESTOPBIT;
		break;
	case 1:
		dcb.StopBits = ONE5STOPBITS;
		break;
	case 2:
		dcb.StopBits = TWOSTOPBITS;
		break;
	default:
		dcb.StopBits = dcb.StopBits;
	}
	return (FALSE != SetCommState(m_hComm, &dcb));
}

size_t TSeriesComm::Read(void *buff, size_t buffSize)
{
	if(!m_connected) return -1;

	DWORD dwErrorFlags = 0;
	ClearCommError(m_hComm, &dwErrorFlags, &m_comStat);

	DWORD length = buffSize;
	length = min(length, m_comStat.cbInQue);
	ReadFile(m_hComm, buff, length, &length, &m_osRead);
	return length;
}

size_t TSeriesComm::Write(void *buff, size_t buffSize)
{
	if(!m_connected) return -1;

	DWORD dwErrorFlags = 0;
	ClearCommError(m_hComm, &dwErrorFlags, &m_comStat);

	DWORD length = buffSize;
	WriteFile(m_hComm, buff, length, &length, &m_osWrite);
	return length;
}

bool TSeriesComm::ClearBuffer( EClearFlag eFlag )
{
	BOOL success = FALSE;
	switch(eFlag)
	{
	case eTxAbort:
		success = PurgeComm(m_hComm, PURGE_TXABORT);
		break;
	case eRxAbort:
		success = PurgeComm(m_hComm, PURGE_RXABORT);
		break;
	case eTxClear:
		success = PurgeComm(m_hComm, PURGE_TXCLEAR);
		break;
	case eRxClear:
		success = PurgeComm(m_hComm, PURGE_RXCLEAR);
		break;
	default:
		success = FALSE;
	}
	return FALSE != success;
}

bool TSeriesComm::FlushBuffer( )
{
	return FALSE != FlushFileBuffers(m_hComm);
}

