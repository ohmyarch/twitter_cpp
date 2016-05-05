/****************************************************************************
* *
** Copyright (C) 2015-2016 Michael Yang
** Contact: ohmyarchlinux@gmail.com
**
** This file is part of the examples of the twitter_cpp.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of the twitter_cpp library nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#pragma once

#include "twitter/client.h"
#include <experimental/optional>
#if defined(_WIN32) && !defined(__cplusplus_winrt)
#include "twitter/utility.h"
#endif

const twitter::string_t consumer_key(u("0eDcIJLNpeeSosBdvNQk5cq3u"));
const twitter::string_t
    consumer_secret(u("zltRuNwj1twFxKBMb22Kz8DSRqgW9W3lxCO9EstlqDCysJc6le"));
const twitter::string_t callback_uri(u("http://127.0.0.1:8890/"));

enum class init_flag : std::uint8_t { error, normal, show_help };

inline bool open_browser(const twitter::string_t &uri) {
#if defined(_WIN32) && !defined(__cplusplus_winrt)
    // NOTE: Windows desktop only.
    auto r = ShellExecuteA(NULL, "open", twitter::utf16_to_utf8(uri).c_str(),
                           NULL, NULL, SW_SHOWNORMAL);

    return true; // FIXME
#elif defined(__APPLE__)
    // NOTE: OS X only.
    std::string browser_cmd("open \"" + uri + "\"");
    std::system(browser_cmd.c_str());

    return true; // FIXME
#else
    // NOTE: Linux/X11 only.
    std::string browser_cmd("xdg-open \"" + uri + "\"");
    std::system(browser_cmd.c_str());

    return true; // FIXME
#endif
}

std::experimental::optional<twitter::twitter_client>
auth(const twitter::string_t &proxy_uri);

std::pair<init_flag, twitter::string_t>
init(const int argc, char **argv, const std::string &application_name);
