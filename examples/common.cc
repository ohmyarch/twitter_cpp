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
#include <boost/program_options.hpp>

std::experimental::optional<twitter::twitter_client>
auth(const twitter::string_t &proxy_uri) {
    twitter::twitter_client client(consumer_key, consumer_secret, u("oob"));
    if (proxy_uri.empty())
        return std::experimental::optional<twitter::twitter_client>();

    if (!client.set_proxy(proxy_uri))
        return std::experimental::optional<twitter::twitter_client>();

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

std::pair<init_flag, twitter::string_t>
init(const int argc, char **argv, const std::string &application_name) {
    try {
        boost::program_options::options_description generic("generic options");
        generic.add_options()("help,h", "print this help");

        boost::program_options::options_description proxy("proxy options");
        proxy.add_options()(
            "proxy-uri,x", boost::program_options::value<twitter::string_t>(),
            "[protocol://]host[:port]  use proxy on given port");

        boost::program_options::options_description input;
        input.add_options()("input",
                            boost::program_options::value<std::string>());

        boost::program_options::options_description visible;
        visible.add(generic).add(proxy);

        boost::program_options::options_description opt_desc;
        opt_desc.add(generic).add(proxy).add(input);

        boost::program_options::positional_options_description pos_opt_desc;
        pos_opt_desc.add("input", 0);

        boost::program_options::variables_map map;
        boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv)
                .options(opt_desc)
                .positional(pos_opt_desc)
                .run(),
            map);
        boost::program_options::notify(map);

        if (map.count("help")) {
            std::cout << "Usage: " + application_name + " [options]"
                      << std::endl
                      << visible;
            return std::make_pair(init_flag::show_help, u(""));
        } else if (map.count("proxy-uri")) {
            return std::make_pair(init_flag::normal,
                                  map["proxy-uri"].as<twitter::string_t>());
        } else {
            return std::make_pair(init_flag::normal, u(""));
            ;
        }
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl
                  << "try '" + application_name +
                         " --help' for more information"
                  << std::endl;

        return std::make_pair(init_flag::error, u(""));
    }
}
