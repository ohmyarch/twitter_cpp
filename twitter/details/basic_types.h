/****************************************************************************
**
** Copyright (C) 2015-2016 Michael Yang
** Contact: ohmyarchlinux@gmail.com
**
** This file is part of the twitter_cpp.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace twitter {
using utf8char = char;
using utf8string = std::string;
using utf8ostream = std::ostream;
using utf8istream = std::istream;
using utf8stringstream = std::stringstream;
using utf8istringstream = std::istringstream;
using utf8ostringstream = std::ostringstream;

#ifdef _WIN32
#define ucin std::wcin
#define ucout std::wcout
#define ucerr std::wcerr
#define u(x) L##x

using char_t = wchar_t;
using string_t = std::wstring;
using ostringstream_t = std::wostringstream;
using ofstream_t = std::wofstream;
using ostream_t = std::wostream;
using istream_t = std::wistream;
using ifstream_t = std::wifstream;
using istringstream_t = std::wistringstream;
using stringstream_t = std::wstringstream;

using utf16char = wchar_t;
using utf16string = std::wstring;
using utf16istream = std::wistream;
using utf16ostream = std::wostream;
using utf16stringstream = std::wstringstream;
using utf16istringstream = std::wistringstream;
using utf16ostringstream = std::wostringstream;
#else
#define ucin std::cin
#define ucout std::cout
#define ucerr std::cerr
#define u(x) x

using char_t = char;
using string_t = std::string;
using istream_t = std::istream;
using ostream_t = std::ostream;
using ifstream_t = std::ifstream;
using ofstream_t = std::ofstream;
using stringstream_t = std::stringstream;
using istringstream_t = std::istringstream;
using ostringstream_t = std::ostringstream;

using utf16char = char16_t;
using utf16string = std::u16string;
using utf16istream = std::basic_istream<utf16char>;
using utf16ostream = std::basic_ostream<utf16char>;
using utf16stringstream = std::basic_stringstream<utf16char>;
using utf16istringstream = std::basic_istringstream<utf16char>;
using utf16ostringstream = std::basic_ostringstream<utf16char>;
#endif
}
