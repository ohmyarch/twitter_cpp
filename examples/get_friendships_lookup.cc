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

#include "common.h"

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    const auto init = ::init(argc, argv, "get_friendships_lookup");
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

    const auto friendships =
        client->get_friendships_lookup(std::vector<twitter::string_t>{
            u("ohmyarch"), u("kassol_zx"), u("twitterapi")});
    if (friendships.empty()) {
        std::cout << "Failed." << std::endl;

        return 1;
    }

    std::size_t index = 0;
    std::size_t size = friendships.size();

    auto print_connections = [](const twitter::connections &connections) {
        if (connections.is_none()) {
            std::cout << "  none" << std::endl;

            return;
        }
        if (connections.is_following())
            std::cout << "  following" << std::endl;
        if (connections.is_following_requested())
            std::cout << "  following_requested" << std::endl;
        if (connections.is_followed_by())
            std::cout << "  followed_by" << std::endl;
        if (connections.is_muting())
            std::cout << "  muting" << std::endl;
        if (connections.is_blocking())
            std::cout << "  blocking" << std::endl;
    };

    for (const auto &e : friendships) {
        ucout << u("name: ") << e.name() << std::endl
              << u("screen_name: ") << e.screen_name() << std::endl
              << u("id: ") << e.id() << std::endl
              << u("id_str: ") << e.id_str() << std::endl
              << u("connections: ") << std::endl;

        print_connections(e.connections());

        if (++index < size)
            std::cout << std::endl;
    }
}
