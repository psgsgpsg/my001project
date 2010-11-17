//////////////////////////////////////////////////////////////////////////
// TString std::string or std::wstring tool
// Created	Date: 2008.04.19
// Update	Date: 2008.04.19

#ifndef TSTRING_H
#define TSTRING_H

#include <TCHAR.h>
#include <string>

#if defined(_UNICODE) || defined(UNICODE)
#define TString  std::wstring
#else
#define TString  std::string
#endif

#endif  //TSTRING_H
