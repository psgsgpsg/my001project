// TLogStream.h  : Interface of the TLogStream class
//
// Copyright (c) 2006 zhao_fsh
// Reference to "Applied C++" book, Philip Romanik and Amy Muntz
//
// Permission to use, copy, modify, distribute, and sell this software and
// its documentation for any purpose is hereby granted without fee, provided
// that (i) the above copyright notices and this permission notice appear in
// all copies of the software and related documentation, and (ii) the names
// of zhao_fsh not be used in any advertising or publicity relating to the
// software without the specific, prior written zhao_fsh.
//
// Use of this software and/or its documentation will be deemed to be
// acceptance of these terms.
//
// Simple log tool

#ifndef TLOGSTREAM_H
#define TLOGSTREAM_H

// General-purpose log stream, like std::cout
#include <ostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>  // for class TLogSinkWindow

// *  TLogSink  *

// TLogSink and derived classes are used to specify the behavior of what
// happens to output sent to the ctlog stream. A TLogStringBuf<>
// object is defined in this file which derives from std::basic_stringbuf.
// This object is used when constructing ctlog (which is a std::ostream
// object). The stream objects can definitely get confusing. The objects
// presented here show an easy way to hook into streams for your own
// purposes. There are more elegant ways but they require more knowledge
// about streams than most people need to worry about.
//
// TLogSink			  Abstract base class that defines a logging stream.
// TLogSinkNull       Does nothing
// TLogSinkConsole    Writes logging information to std::cout
// TLogSinkFile       Writes logging information to the specified file.
// TLogSinkWindow	  Writes logging information to the designation window
// Common overrides (of TLogSink)
//
//   header(). Override to change the standard header that is displayed

class TLogSink
{
public:
  TLogSink ();

  virtual void Write (const std::string& str) = 0;
  // Write a string to our logging sink;

  virtual void Write (int c) = 0;
  // Write a character to our logging sink. Usually called from
  // a std::basic_stringbuf derived object as characters are received.

  virtual void Flush () {}
  // flush any stored information if possible for this type of sink.

  virtual std::string Header () { return StandardHeader();}
  // By default, we emit our standard header when headers are enabled

  void ShowHeader (bool state) {  m_enableHeader = state;}
  // Set the state of whether a header is written

protected:
  std::string StandardHeader ();
  // The standard header consists of the current time

  bool m_enableHeader; // true to Write header whenever buffer is flushed
};


// *  TLogSinkNull  *
// Null sink

class TLogSinkNull : public TLogSink
{
public:
	static TLogSinkNull& Instance() { return m_Only; }

  virtual void Write (const std::string& /*str*/) {}
  virtual void Write (int /*c*/) {}

private:
  static TLogSinkNull m_Only;
  TLogSinkNull ();
};

// *  TLogSinkConsole  *

// Console sink. Data is written to std::cout
//
// Common overrides
//
//   display(). Override to change the output source. Default is std::cout.


class TLogSinkConsole : public TLogSink
{
public:
	static TLogSinkConsole& Instance() { return m_Only; }
  virtual void Write (const std::string& str);
  virtual void Write (int c);
  virtual void Flush ();
  virtual ~TLogSinkConsole ();
protected:
  virtual void Display (const std::string& str);
  // Output the string. Derived classes can override this

  TLogSinkConsole ();
  static TLogSinkConsole m_Only;
  std::string m_buffer;
};

// *  TLogSinkFile  *

// File sink

class TLogSinkFile : public TLogSinkConsole
{
public:
	static TLogSinkFile& Instance() { return m_Only; }

  void SetFile (const std::string& file);
  // Set/change our file name. The stream is flushed before the
  // file name is changed.
  virtual ~TLogSinkFile ();
private:
  virtual void Display (const std::string& str);

  TLogSinkFile ();

  static TLogSinkFile m_Only;
  std::string m_file;
};

// *  TLogSinkWindow  *

// Window sink
class TLogSinkWindow : public TLogSinkConsole
{
public:
	static TLogSinkWindow& Instance() { return m_Only; }

  void SetHWND (const HWND& hWnd);
  // Set/change window's handle. The stream is flushed before the
  // window's handle is changed.
  virtual ~TLogSinkWindow ();
private:
  virtual void Display (const std::string& str);

  TLogSinkWindow ();

  static TLogSinkWindow m_Only;
  HWND   m_hWnd;
};

// *  TLogStringBuf  *

// We need to derive a class from std::basic_stringbuf<> to intercept
// characters for our TLog sinks. TLogStringBuf bridges the
// gap by intercepting stream characters by overriding overflow() and
// passing the characters to the current sink

template< class T, class Tr = std::char_traits<T>, class A = std::allocator<T> >
class TLogBuf : public std::basic_stringbuf<T, Tr, A>
{
public:
  TLogBuf (TLogSink& s = TLogSinkNull::Instance()) : m_sink (&s) {}
  ~TLogBuf () { m_sink->Flush();}

  typedef typename std::basic_stringbuf<T, Tr, A>::int_type int_type;

  int_type overflow (int_type c) {
    if (c != traits_type::eof())
      m_sink->Write (c);
    return traits_type::not_eof(c);
  }

  void Sink (TLogSink& s) { m_sink = &s;}
  // Change our sink

private:
  TLogBuf (const TLogBuf&);  // Do not allow copies
  TLogBuf& operator= (const TLogBuf&);  // Do not allow copies

  TLogSink* m_sink;
};


// Our global instance we will use to Write debugging information
// You can use cdebug just like std::cout. The default sink is 
// TLogSinkNull so you'll need to set it to something, before
// using ctlog, ex:
//     tlogstream.sink (apDebugSinkConsole::sOnly);

extern TLogBuf<char> tlogstream;
extern std::ostream ctlog;


#endif // _debugstream_h_
