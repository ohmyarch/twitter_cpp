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

#include "twitter/details/return_types.h"
#include <cpprest/http_client.h>
#include <cpprest/oauth1.h>
#include <experimental/optional>

namespace twitter {
// enum class error : std::uint8_t {};

class token {
  public:
    token();
    token(const string_t &access_token, const string_t &secret)
        : access_token_(access_token), secret_(secret) {}
    token(string_t &&access_token, string_t &&secret)
        : access_token_(std::move(access_token)), secret_(std::move(secret)) {}
    token(token &&other) noexcept
        : access_token_(std::move(other.access_token_)),
          secret_(std::move(other.secret_)) {}

    void set_access_token(const string_t &access_token) {
        access_token_ = access_token;
    }
    void set_access_token(std::string &&access_token) {
        access_token_ = std::move(access_token);
    }
    void set_secret(const string_t &secret) { secret_ = secret; }
    void set_secret(string_t &&secret) { secret_ = std::move(secret); }

    const string_t &access_token() const { return access_token_; }
    const string_t &secret() const { return secret_; }

    bool is_valid_access_token() const {
        return !(access_token_.empty() || secret_.empty());
    }

    token &operator=(const token &other) {
        if (&other != this) {
            access_token_ = other.access_token_;
            secret_ = other.secret_;
        }

        return *this;
    }
    token &operator=(token &&other) noexcept {
        if (&other != this) {
            access_token_ = std::move(other.access_token_);
            secret_ = std::move(other.secret_);
        }

        return *this;
    }
    bool operator==(const token &other) const {
        return (access_token_ == other.access_token_) &&
               (secret_ == other.secret_);
    }
    bool operator!=(const token &other) const {
        return (access_token_ != other.access_token_) ||
               (secret_ != other.secret_);
    }

  private:
    friend class twitter_client;

    string_t access_token_;
    string_t secret_;
};

class twitter_client {
  public:
    twitter_client(const string_t &consumer_key,
                   const string_t &consumer_secret,
                   const string_t &callback_uri);
    twitter_client(string_t &&consumer_key, string_t &&consumer_secret,
                   string_t &&callback_uri);

    void set_token(const token &token) {
        oauth1_config_.set_token(web::http::oauth1::experimental::oauth1_token(
            token.access_token_, token.secret_));

        http_client_config_.set_oauth1(oauth1_config_);
    }
    void set_token(token &&token) {
        oauth1_config_.set_token(web::http::oauth1::experimental::oauth1_token(
            std::move(token.access_token_), std::move(token.secret_)));

        http_client_config_.set_oauth1(oauth1_config_);
    }
    bool set_proxy(const string_t &proxy_uri) {
        try {
            web::web_proxy proxy(proxy_uri);

            oauth1_config_.set_proxy(proxy);
            http_client_config_.set_proxy(proxy);

            return true;
        } catch (const web::uri_exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        return false;
    }
    void set_consumer_key(const string_t &consumer_key) {
        oauth1_config_.set_consumer_key(consumer_key);
    }
    void set_consumer_key(string_t &&consumer_key) {
        oauth1_config_.set_consumer_key(std::move(consumer_key));
    }
    void set_consumer_secret(const string_t &consumer_secret) {
        oauth1_config_.set_consumer_secret(consumer_secret);
    }
    void set_consumer_secret(string_t &&consumer_secret) {
        oauth1_config_.set_consumer_secret(std::move(consumer_secret));
    }
    void set_callback_uri(const string_t &callback_uri) {
        oauth1_config_.set_callback_uri(callback_uri);
    }
    void set_callback_uri(string_t &&callback_uri) {
        oauth1_config_.set_callback_uri(std::move(callback_uri));
    }

    const string_t &consumer_key() const {
        return oauth1_config_.consumer_key();
    }
    const string_t &consumer_secret() const {
        return oauth1_config_.consumer_secret();
    }
    const string_t &callback_uri() const {
        return oauth1_config_.callback_uri();
    }
    class token token() const {
        const web::http::oauth1::experimental::oauth1_token &oauth1_token =
            oauth1_config_.token();

        return twitter::token(oauth1_token.access_token(),
                              oauth1_token.secret());
    }

    // void listen_for_code();
    string_t build_authorization_uri();
    bool token_from_pin(const string_t &pin);
    bool token_from_pin(string_t &&pin);

    // void get_friendships_show(const string_t &source_screen_name,
    // const string_t &target_screen_name);
    // void get_friendships_show(const std::uint64_t &source_user_id,
    // const string_t &target_screen_name);
    // void get_friendships_show(const string_t &source_screen_name,
    // const std::uint64_t &target_user_id);
    // void get_friendships_show(const std::uint64_t &source_user_id,
    // const std::uint64_t &target_user_id);
    std::vector<friendship>
    get_friendships_lookup(const std::vector<string_t> &screen_names) const;
    std::vector<friendship>
    get_friendships_lookup(const std::vector<std::uint64_t> &user_ids) const;
    // void get_friendships_lookup(const std::vector<user> &users);

    std::experimental::optional<account_settings> get_account_settings() const;

    std::vector<suggested_category>
    get_users_suggestions(language lang = language::en) const;

    std::experimental::optional<configuration> get_help_configuration() const;
    std::vector<language_info> get_help_languages() const;
    string_t get_help_privacy() const;
    string_t get_help_tos() const;

  protected:
    web::http::client::http_client_config http_client_config_;
    web::http::oauth1::experimental::oauth1_config oauth1_config_;
};
}
