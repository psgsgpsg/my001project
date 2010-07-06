// TLog.h  : Interface of the TLog class
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
// Simple control log tool
#pragma warning (disable : 4786 4146)

#ifndef TLOG_H
#define TLOG_H

#include <bitset>

// LOGGING(mode,statement) is used to define one or more c++
// statements to run when the current Logging mode is one of the
// specified mode. You can generate production releases with this
// code enabled, or you can #define NOLOG to compile-out the code.
//
// For single line statements you might try this:
//   LOGGING(1, ctlog << "Hello World" << std::endl;)
//
// For longer statements, this looks more natrual:
//   LOGGING(1, {
//     for (int i=0; i<10; i++)
//       ctlog << "Hello World" << std::endl;
//   })
//

#ifdef NOLOG
#define LOGGING(mode,statements)
#else
#define LOGGING(mode,statements)		   \
	if (TLog::Instance().IsLog(mode)) {	   \
    statements                             \
	}
#endif

class TLog
{
public:
	typedef enum
	{
		eNone			= 0x00,
		eDcmServer		= 0x01,
		eDatabase		= 0x02,
		eGUI			= 0x04,
		eDebug			= 0x08,
		eExeption		= 0x10,
		eError			= 0x20,
		eAll			= 0x3F,
	} EnumLog;
public:
	static TLog& Instance ();
	bool IsLog	(int mode = eAll);
	void Set	(int mode = eAll);
	void Clear	(int mode = eAll);
	
private:
	enum	{ N =32	};	// 32 is an arbitrary value, you can raise it or lower it
	static TLog		*m_pInstance;
	std::bitset<N>	m_log;			
	
	TLog ();
};

#endif // TLOG_H
