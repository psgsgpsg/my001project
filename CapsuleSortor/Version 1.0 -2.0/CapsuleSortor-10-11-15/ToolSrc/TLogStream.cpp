// TLogStream.cpp

// General-purpose debug stream, like std::cout

#include "TLogStream.h"
#include <time.h>	// for time(); ctime();


// *  TLogSink  *

TLogSink::TLogSink () : m_enableHeader(false) {}

std::string TLogSink::StandardHeader ()
{
  std::string header;

  // Fetch the current time
  time_t now = time(0);
  header += ctime (&now);
  header.erase (header.length()-1, 1); // Remove newline written
  header += ": ";

  return header;
}


// *  TLogSinkNull  *

TLogSinkNull TLogSinkNull::m_Only = TLogSinkNull ();

TLogSinkNull::TLogSinkNull () {}


// *  apDebugSinkConsole  *

TLogSinkConsole TLogSinkConsole::m_Only = TLogSinkConsole();

TLogSinkConsole::TLogSinkConsole  () {}
TLogSinkConsole::~TLogSinkConsole () { Flush();}


void TLogSinkConsole::Write (const std::string& str)
{
  m_buffer += str;
  Flush ();
}

void TLogSinkConsole::Write (int c)
{
  m_buffer += (char)c;
  if (c == '\n')
    Flush ();
}

void TLogSinkConsole::Display (const std::string& str)
{
  std::cout << str;
}

void TLogSinkConsole::Flush ()
{
  if (m_buffer.empty())
    return;

  if (m_enableHeader)
    m_buffer = Header() + m_buffer;

  // Add a trailing newline if we don't have one
  if (m_buffer[m_buffer.length()-1] != '\n')
    m_buffer += '\n';

  Display (m_buffer);
  m_buffer.erase ();
}

// *  TLogSinkFile  *


TLogSinkFile TLogSinkFile::m_Only = TLogSinkFile ();

TLogSinkFile::TLogSinkFile  () {}
TLogSinkFile::~TLogSinkFile () { Flush();}

void TLogSinkFile::SetFile (const std::string& file)
{
  Flush(); 
  m_file = file;
}

void TLogSinkFile::Display (const std::string& str)
{
  if (m_file.empty())
    return;

  // Open the file in append mode. The dtor will close
  // the file for us.
  std::ofstream output (m_file.c_str(), std::ios_base::app);
  if (!output)
    return;    // The file could not be opened. Exit

  output << str;
}

// *  TLogSinkWindow  *


TLogSinkWindow TLogSinkWindow::m_Only = TLogSinkWindow ();

TLogSinkWindow::TLogSinkWindow  () {}
TLogSinkWindow::~TLogSinkWindow () { Flush();}

void TLogSinkWindow::SetHWND (const HWND& hWnd)
{
  Flush(); 
  m_hWnd = hWnd;
}

void TLogSinkWindow::Display (const std::string& str)
{
	static int startRowPos =0;
	if (NULL == m_hWnd)
		return;
	
	HDC hDC = ::GetDC(m_hWnd);
	if( NULL == hDC )
		return;
	int textHeight =0;
	{
		TEXTMETRIC tm;
		memset(&tm,	0,	sizeof(tm));
		::GetTextMetrics(hDC, &tm);
		textHeight = tm.tmHeight + tm.tmExternalLeading; 
	}

	std::string textLine;
	std::string::size_type prevpos =0, pos = 0;
	while ( (pos = str.find_first_of('\n',pos)) != std::string::npos )
	{
		textLine = str.substr(prevpos, pos -prevpos);
		prevpos = ++pos;
		::TextOut(hDC,0, startRowPos, textLine.c_str(), textLine.length());
		startRowPos += textHeight;
	}
	::ReleaseDC(m_hWnd,hDC);
}


// *  TLogStringBuf  *

TLogBuf<char> tlogstream;
std::ostream ctlog(&tlogstream);

