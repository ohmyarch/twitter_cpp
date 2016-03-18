/****************************************************************************
**
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

#include "common.h"
#include <boost/lexical_cast.hpp>

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    const auto init = ::init(argc, argv, "get_account_settings");
    init_flag flag = std::get<0>(init);
    if (flag == init_flag::error)
        return 1;
    else if (flag == init_flag::show_help)
        return 0;

    const auto client = auth(std::get<1>(init));
    if (!client) {
        std::cout << "Failed." << std::endl;

        return 1;
    }

    auto allowed_to_string = [](const twitter::allowed who) {
        if (who == twitter::allowed::all)
            return u("all");
        else if (who == twitter::allowed::following)
            return u("following");
        else
            return u("none");
    };

    auto hour_to_string = [](const twitter::hour hour) {
        std::int8_t i;

        if ((i = static_cast<std::int8_t>(hour)) == -1)
            return twitter::string_t(u("null"));
        else
            return boost::lexical_cast<twitter::string_t>(i);
    };

    auto language_to_string = [](const twitter::language lang) {
        switch (lang) {
        case twitter::language::fr:
            return u("fr");
        case twitter::language::en:
            return u("en");
        case twitter::language::ar:
            return u("ar");
        case twitter::language::ja:
            return u("ja");
        case twitter::language::es:
            return u("es");
        case twitter::language::de:
            return u("de");
        case twitter::language::it:
            return u("it");
        case twitter::language::id:
            return u("id");
        case twitter::language::pt:
            return u("pt");
        case twitter::language::ko:
            return u("ko");
        case twitter::language::tr:
            return u("tr");
        case twitter::language::ru:
            return u("ru");
        case twitter::language::nl:
            return u("nl");
        case twitter::language::fil:
            return u("fil");
        case twitter::language::msa:
            return u("msa");
        case twitter::language::zh_tw:
            return u("zh-TW");
        case twitter::language::zh_cn:
            return u("zh-CN");
        case twitter::language::hi:
            return u("hi");
        case twitter::language::no:
            return u("no");
        case twitter::language::sv:
            return u("sv");
        case twitter::language::fi:
            return u("fi");
        case twitter::language::da:
            return u("da");
        case twitter::language::pl:
            return u("pl");
        case twitter::language::hu:
            return u("hu");
        case twitter::language::fa:
            return u("fa");
        case twitter::language::he:
            return u("he");
        case twitter::language::th:
            return u("th");
        case twitter::language::uk:
            return u("uk");
        case twitter::language::cs:
            return u("cs");
        case twitter::language::ro:
            return u("ro");
        case twitter::language::en_gb:
            return u("en-GB");
        case twitter::language::vi:
            return u("vi");
        case twitter::language::bn:
            return u("bn");
        }
    };

    auto settings = client->get_account_settings();
    if (!settings) {
        std::cout << "Failed." << std::endl;

        return 1;
    }

    ucout << std::boolalpha << u("Settings for the authenticating user:")
          << std::endl
          << std::endl
          << u("time_zone:") << std::endl
          << u("  name: ") << settings->time_zone().name() << std::endl
          << u("  utc_offset: ") << settings->time_zone().utc_offset()
          << std::endl
          << u("  tzinfo_name: ") << settings->time_zone().tzinfo_name()
          << std::endl
          << u("protected: ") << settings->is_protected() << std::endl
          << u("screen_name: ") << settings->screen_name() << std::endl
          << u("always_use_https: ") << settings->is_always_use_https()
          << std::endl
          << u("use_cookie_personalization: ")
          << settings->is_use_cookie_personalization() << std::endl
          << "sleep_time:" << std::endl
          << "  enabled: " << settings->sleep_time().is_enabled() << std::endl
          << "  start_time: "
          << hour_to_string(settings->sleep_time().start_time()) << std::endl
          << "  end_time: " << hour_to_string(settings->sleep_time().end_time())
          << std::endl
          << u("geo_enabled: ") << settings->is_geo_enabled() << std::endl
          << u("language: ") << language_to_string(settings->language())
          << std::endl
          << u("discoverable_by_email: ")
          << settings->is_discoverable_by_email() << std::endl
          << u("discoverable_by_mobile_phone: ")
          << settings->is_discoverable_by_mobile_phone() << std::endl
          << u("display_sensitive_media: ")
          << settings->is_display_sensitive_media() << std::endl
          << u("allow_contributor_request: ")
          << allowed_to_string(settings->allow_contributor_request())
          << std::endl
          << u("allow_dms_from: ")
          << allowed_to_string(settings->allow_dms_from()) << std::endl
          << u("allow_dm_groups_from: ")
          << allowed_to_string(settings->allow_dm_groups_from()) << std::endl
          << u("smart_mute: ") << settings->smart_mute() << std::endl;
}
