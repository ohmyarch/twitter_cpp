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

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    const auto init = ::init(argc, argv, "get_help_configuration");
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

    const auto config = client->get_help_configuration();
    if (!config) {
        std::cout << "Failed." << std::endl;

        return 1;
    }

    ucout << u("The current configuration used by Twitter:") << std::endl
          << std::endl
          << u("dm_text_character_limit: ") << config->dm_text_character_limit()
          << std::endl
          << u("characters_reserved_per_media: ")
          << config->characters_reserved_per_media() << std::endl
          << u("max_media_per_upload: ") << config->max_media_per_upload()
          << std::endl
          << u("short_url_length: ") << config->short_url_length() << std::endl
          << u("short_url_length_https: ") << config->short_url_length_https()
          << std::endl
          << u("photo_size_limit: ") << config->photo_size_limit() << std::endl
          << u("photo_sizes:") << std::endl
          << u("  thumb:") << std::endl
          << u("    h: ") << config->thumb_photo_size().h() << std::endl
          << u("    w: ") << config->thumb_photo_size().w() << std::endl
          << u("    resize: ")
          << ((config->thumb_photo_size().resize() == twitter::resize::fit)
                  ? u("fit")
                  : u("crop"))
          << std::endl
          << u("  small:") << std::endl
          << u("    h: ") << config->small_photo_size().h() << std::endl
          << u("    w: ") << config->small_photo_size().w() << std::endl
          << u("    resize: ")
          << ((config->small_photo_size().resize() == twitter::resize::fit)
                  ? u("fit")
                  : u("crop"))
          << std::endl
          << u("  medium:") << std::endl
          << u("    h: ") << config->medium_photo_size().h() << std::endl
          << u("    w: ") << config->medium_photo_size().w() << std::endl
          << u("    resize: ")
          << ((config->medium_photo_size().resize() == twitter::resize::fit)
                  ? u("fit")
                  : u("crop"))
          << std::endl
          << u("  large:") << std::endl
          << u("    h: ") << config->large_photo_size().h() << std::endl
          << u("    w: ") << config->large_photo_size().w() << std::endl
          << u("    resize: ")
          << ((config->large_photo_size().resize() == twitter::resize::fit)
                  ? u("fit")
                  : u("crop"))
          << std::endl
          << u("non_username_paths:") << std::endl;

    for (const auto &e : config->non_username_paths())
        ucout << u("                    ") << e << std::endl;
}
