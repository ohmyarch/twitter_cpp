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
**   * Neither the name of The Qt Company Ltd nor the names of its
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
    auto client = auth();
    if (!client) {
        std::cout << "Failed." << std::endl;

        return 0;
    }

    auto allowed_to_string = [](const twitter::allowed who) {
        if (who == twitter::allowed::all)
            return u("all");
        else if (who == twitter::allowed::following)
            return u("following");
        else
            return u("none");
    };

    int i;
    auto hour_to_string = [&i](const twitter::hour hour) {
        if ((i = static_cast<int>(hour)) == -1)
            return twitter::string_t(u("null"));
        else
            return boost::lexical_cast<twitter::string_t>(i);
    };

    auto settings = client->get_account_settings();
    if (!settings) {
        std::cout << "Failed." << std::endl;

        return 0;
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
          << u("language: ") << settings->language() << std::endl
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
