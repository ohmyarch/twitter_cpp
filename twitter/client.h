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

#include "twitter/details/basic_types.h"
#include <cpprest/http_client.h>
#include <cpprest/oauth1.h>
class string_t;

namespace twitter {
// enum class error : std::uint8_t {};

class token {
  public:
    token(const string_t &access_token, const string_t &secret)
        : access_token_(access_token), secret_(secret) {}

    void set_access_token(const string_t &access_token) {
        access_token_ = access_token;
    }
    void set_secret(const string_t &secret) { secret_ = secret; }

    string_t access_token() { return access_token_; }
    string_t secret() { return secret_; }

    bool valid_access_token() {
        return !(access_token_.empty() || secret_.empty());
    }

  private:
    string_t access_token_;
    string_t secret_;
};

class twitter_client {
  public:
    twitter_client(const string_t &consumer_key,
                   const string_t &consumer_secret,
                   const string_t &callback_uri);

    void set_proxy(const string_t &proxy_uri) {
        web::web_proxy proxy(proxy_uri);
        oauth1_config_.set_proxy(proxy);
        http_client_config_.set_proxy(proxy);
    }
    void set_consumer_key(const string_t &consumer_key) {
        oauth1_config_.set_consumer_key(consumer_key);
    }
    void set_consumer_secret(const string_t &consumer_secret) {
        oauth1_config_.set_consumer_secret(consumer_secret);
    }
    void set_callback_uri(const string_t &callback_uri) {
        oauth1_config_.set_callback_uri(callback_uri);
    }

    string_t consumer_key() const { return oauth1_config_.consumer_key(); }
    string_t consumer_secret() const {
        return oauth1_config_.consumer_secret();
    }
    string_t callback_uri() const { return oauth1_config_.callback_uri(); }
    twitter::token token() const {
        web::http::oauth1::experimental::oauth1_token oauth1_token =
            oauth1_config_.token();

        return twitter::token(oauth1_token.access_token(),
                              oauth1_token.secret());
    }

    void listen_for_code();
    string_t build_authorization_uri();
    bool token_from_pin(const string_t &pin);

    string_t get_account_settings() const;

  protected:
    web::http::client::http_client_config http_client_config_;
    web::http::oauth1::experimental::oauth1_config oauth1_config_;
};
}
