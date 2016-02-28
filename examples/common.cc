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

std::experimental::optional<twitter::twitter_client> auth() {
    twitter::twitter_client client(consumer_key, consumer_secret, u("oob"));
    client.set_proxy(proxy_uri);

    twitter::string_t access_token;
    twitter::string_t access_token_secret;

    {
        twitter::ifstream_t access_token_file("access_token");
        twitter::ifstream_t access_token_secret_file("access_token_secret");

        if (access_token_file.is_open() && access_token_secret_file.is_open()) {
            access_token_file >> access_token;
            access_token_secret_file >> access_token_secret;
        }
    }

    twitter::token token(access_token, access_token_secret);
    if (token.is_valid_access_token()) {
        client.set_token(token);
    } else {
        twitter::string_t &&auth_uri = client.build_authorization_uri();

        if (auth_uri.empty())
            return std::experimental::optional<twitter::twitter_client>();

        open_browser(auth_uri);

        std::cout << "Enter the PIN provided by twitter: ";

        twitter::string_t pin;
        ucin >> pin;

        if (!client.token_from_pin(pin))
            return std::experimental::optional<twitter::twitter_client>();

        token = client.token();

        twitter::ofstream_t access_token_file("access_token");
        twitter::ofstream_t access_token_secret_file("access_token_secret");

        access_token_file << token.access_token();
        access_token_secret_file << token.secret();
    }

    return std::experimental::make_optional(client);
}