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

#include "twitter/client.h"
#include <boost/lexical_cast.hpp>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

namespace twitter {
twitter_client::twitter_client(const string_t &consumer_key,
                               const string_t &consumer_secret,
                               const string_t &callback_uri)
    : oauth1_config_(
          consumer_key, consumer_secret,
          u("https://api.twitter.com/oauth/request_token"),
          u("https://api.twitter.com/oauth/authorize"),
          u("https://api.twitter.com/oauth/access_token"), callback_uri,
          web::http::oauth1::experimental::oauth1_methods::hmac_sha1) {}
twitter_client::twitter_client(string_t &&consumer_key,
                               string_t &&consumer_secret,
                               string_t &&callback_uri)
    : oauth1_config_(
          std::move(consumer_key), std::move(consumer_secret),
          u("https://api.twitter.com/oauth/request_token"),
          u("https://api.twitter.com/oauth/authorize"),
          u("https://api.twitter.com/oauth/access_token"),
          std::move(callback_uri),
          web::http::oauth1::experimental::oauth1_methods::hmac_sha1) {}

/* void twitter_client::listen_for_code() {
    auto listener =
        std::make_unique<web::http::experimental::listener::http_listener>(
            web::http::uri(oauth1_config_.callback_uri()));
    listener->support([this](web::http::http_request request) {
        request.request_uri().path();
    });
    listener->open().wait();
} */

string_t twitter_client::build_authorization_uri() {
    auto auth_uri_task = oauth1_config_.build_authorization_uri();
    try {
        return auth_uri_task.get();
    } catch (const web::http::oauth1::experimental::oauth1_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return u("");
}

bool twitter_client::token_from_pin(const string_t &pin) {
    try {
        oauth1_config_.token_from_verifier(pin).get();
    } catch (const web::http::oauth1::experimental::oauth1_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;

        return false;
    }

    http_client_config_.set_oauth1(oauth1_config_);

    return true;
}

bool twitter_client::token_from_pin(string_t &&pin) {
    try {
        oauth1_config_.token_from_verifier(std::move(pin)).get();
    } catch (const web::http::oauth1::experimental::oauth1_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;

        return false;
    }

    http_client_config_.set_oauth1(oauth1_config_);

    return true;
}

std::vector<friendship> twitter_client::get_friendships_lookup(
    const std::vector<string_t> &screen_names) const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    string_t query;
    for (const auto &e : screen_names)
        query += (e + web::uri::encode_data_string(u(",")));

    query.resize(query.size() - 3);

    try {
        web::uri_builder builder(u("friendships/lookup.json"));
        builder.append_query(u("screen_name"), query, false);

        const web::json::array root =
            api.request(web::http::methods::GET, builder.to_string())
                .get()
                .extract_json()
                .get()
                .as_array();

        std::vector<friendship> friendships;

        friendship friendship;

        for (const auto &e : root) {
            const web::json::object &object = e.as_object();
            friendship.name_ = object.at(u("name")).as_string();
            friendship.screen_name_ = object.at(u("screen_name")).as_string();
            friendship.id_ = object.at(u("id")).as_number().to_uint64();
            friendship.id_str_ = object.at(u("id_str")).as_string();

            const web::json::array &connections =
                object.at(u("connections")).as_array();
            for (const auto &connection : connections) {
                const string_t &connection_type = connection.as_string();
                if (connection_type == u("none")) {
                    friendship.connections_.none_ = true;

                    break;
                } else if (connection_type == u("following")) {
                    friendship.connections_.following_ = true;
                } else if (connection_type == u("following_requested")) {
                    friendship.connections_.following_requested_ = true;
                } else if (connection_type == u("followed_by")) {
                    friendship.connections_.followed_by_ = true;
                } else if (connection_type == u("blocking")) {
                    friendship.connections_.blocking_ = true;
                } else if (connection_type == u("muting")) {
                    friendship.connections_.muting_ = true;
                }
            }

            friendships.emplace_back(std::move(friendship));
        }

        return friendships;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return std::vector<friendship>();
}

std::vector<friendship> twitter_client::get_friendships_lookup(
    const std::vector<std::uint64_t> &user_ids) const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    string_t query;
    for (const auto &e : user_ids)
        query += (boost::lexical_cast<string_t>(e) +
                  web::uri::encode_data_string(u(",")));

    query.resize(query.size() - 3);

    try {
        web::uri_builder builder(u("friendships/lookup.json"));
        builder.append_query(u("user_id"), query, false);

        const web::json::array root =
            api.request(web::http::methods::GET, builder.to_string())
                .get()
                .extract_json()
                .get()
                .as_array();

        std::vector<friendship> friendships;

        friendship friendship;

        for (const auto &e : root) {
            const web::json::object &object = e.as_object();
            friendship.name_ = object.at(u("name")).as_string();
            friendship.screen_name_ = object.at(u("screen_name")).as_string();
            friendship.id_ = object.at(u("id")).as_number().to_uint64();
            friendship.id_str_ = object.at(u("id_str")).as_string();

            const web::json::array &connections =
                object.at(u("connections")).as_array();
            for (const auto &connection : connections) {
                const string_t &connection_type = connection.as_string();
                if (connection_type == u("none")) {
                    friendship.connections_.none_ = true;

                    break;
                } else if (connection_type == u("following")) {
                    friendship.connections_.following_ = true;
                } else if (connection_type == u("following_requested")) {
                    friendship.connections_.following_requested_ = true;
                } else if (connection_type == u("followed_by")) {
                    friendship.connections_.followed_by_ = true;
                } else if (connection_type == u("blocking")) {
                    friendship.connections_.blocking_ = true;
                } else if (connection_type == u("muting")) {
                    friendship.connections_.muting_ = true;
                }
            }

            friendships.emplace_back(std::move(friendship));
        }

        return friendships;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return std::vector<friendship>();
}

std::experimental::optional<account_settings>
twitter_client::get_account_settings() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::object root =
            api.request(web::http::methods::GET, u("account/settings.json"))
                .get()
                .extract_json()
                .get()
                .as_object();

        account_settings settings;

        const web::json::object &time_zone =
            root.at(u("time_zone")).as_object();
        settings.time_zone_.name_ = time_zone.at(u("name")).as_string();
        settings.time_zone_.utc_offset_ =
            time_zone.at(u("utc_offset")).as_integer();
        settings.time_zone_.tzinfo_name_ =
            time_zone.at(u("tzinfo_name")).as_string();

        settings.protected_ = root.at(u("protected")).as_bool();

        settings.screen_name_ = root.at(u("screen_name")).as_string();

        settings.always_use_https_ = root.at(u("always_use_https")).as_bool();

        settings.use_cookie_personalization_ =
            root.at(u("use_cookie_personalization")).as_bool();

        const web::json::object &sleep_time =
            root.at(u("sleep_time")).as_object();
        if (sleep_time.at(u("enabled")).as_bool()) {
            settings.sleep_time_.start_time_ =
                static_cast<hour>(sleep_time.at(u("start_time")).as_integer());
            settings.sleep_time_.end_time_ =
                static_cast<hour>(sleep_time.at(u("end_time")).as_integer());
        }

        settings.geo_enabled_ = root.at(u("geo_enabled")).as_bool();

        settings.language_ = root.at(u("language")).as_string();

        settings.discoverable_by_email_ =
            root.at(u("discoverable_by_email")).as_bool();

        settings.discoverable_by_mobile_phone_ =
            root.at(u("discoverable_by_mobile_phone")).as_bool();
        ;

        settings.display_sensitive_media_ =
            root.at(u("display_sensitive_media")).as_bool();

        settings.smart_mute_ = root.at(u("smart_mute")).as_bool();

        auto string_to_allowed = [](const string_t &str) {
            if (str == u("all"))
                return allowed::all;
            else if (str == u("following"))
                return allowed::following;
            else
                return allowed::none;
        };

        settings.allow_contributor_request_ = string_to_allowed(
            root.at(u("allow_contributor_request")).as_string());

        settings.allow_dms_from_ =
            string_to_allowed(root.at(u("allow_dms_from")).as_string());

        settings.allow_dm_groups_from_ =
            string_to_allowed(root.at(u("allow_dm_groups_from")).as_string());

        return std::experimental::make_optional(std::move(settings));
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return std::experimental::optional<account_settings>();
}

std::experimental::optional<configuration>
twitter_client::get_help_configuration() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::object root =
            api.request(web::http::methods::GET, u("help/configuration.json"))
                .get()
                .extract_json()
                .get()
                .as_object();

        configuration config;

        config.dm_text_character_limit_ = static_cast<std::uint16_t>(
            root.at(u("dm_text_character_limit")).as_integer());

        config.characters_reserved_per_media_ = static_cast<std::uint16_t>(
            root.at(u("characters_reserved_per_media")).as_integer());

        config.max_media_per_upload_ = static_cast<std::uint16_t>(
            root.at(u("max_media_per_upload")).as_integer());

        config.short_url_length_ = static_cast<std::uint16_t>(
            root.at(u("short_url_length")).as_integer());

        config.short_url_length_https_ = static_cast<std::uint16_t>(
            root.at(u("short_url_length_https")).as_integer());

        config.photo_size_limit_ =
            root.at(u("photo_size_limit")).as_number().to_uint32();

        const web::json::object &photo_sizes =
            root.at(u("photo_sizes")).as_object();

        const web::json::object &thumb = photo_sizes.at(u("thumb")).as_object();
        config.thumb_photo_size_.h_ =
            static_cast<std::uint16_t>(thumb.at(u("h")).as_integer());
        config.thumb_photo_size_.w_ =
            static_cast<std::uint16_t>(thumb.at(u("w")).as_integer());
        config.thumb_photo_size_.resize_ =
            (thumb.at(u("resize")).as_string() == u("fit")) ? resize::fit
                                                            : resize::crop;

        const web::json::object &small = photo_sizes.at(u("small")).as_object();
        config.small_photo_size_.h_ =
            static_cast<std::uint16_t>(small.at(u("h")).as_integer());
        config.small_photo_size_.w_ =
            static_cast<std::uint16_t>(small.at(u("w")).as_integer());
        config.small_photo_size_.resize_ =
            (small.at(u("resize")).as_string() == u("fit")) ? resize::fit
                                                            : resize::crop;

        const web::json::object &medium =
            photo_sizes.at(u("medium")).as_object();
        config.medium_photo_size_.h_ =
            static_cast<std::uint16_t>(medium.at(u("h")).as_integer());
        config.medium_photo_size_.w_ =
            static_cast<std::uint16_t>(medium.at(u("w")).as_integer());
        config.medium_photo_size_.resize_ =
            (medium.at(u("resize")).as_string() == u("fit")) ? resize::fit
                                                             : resize::crop;

        const web::json::object &large = photo_sizes.at(u("large")).as_object();
        config.large_photo_size_.h_ =
            static_cast<std::uint16_t>(large.at(u("h")).as_integer());
        config.large_photo_size_.w_ =
            static_cast<std::uint16_t>(large.at(u("w")).as_integer());
        config.large_photo_size_.resize_ =
            (large.at(u("resize")).as_string() == u("fit")) ? resize::fit
                                                            : resize::crop;

        const web::json::array &non_username_paths =
            root.at(u("non_username_paths")).as_array();
        for (const auto &e : non_username_paths) {
            config.non_username_paths_.emplace_back(e.as_string());
        }

        return std::experimental::make_optional(std::move(config));
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return std::experimental::optional<configuration>();
}

std::vector<language> twitter_client::get_help_languages() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::array root =
            api.request(web::http::methods::GET, u("help/languages.json"))
                .get()
                .extract_json()
                .get()
                .as_array();

        std::vector<language> languages;

        for (const auto &e : root) {
            const web::json::object &object = e.as_object();
            const string_t &code = object.at(u("code")).as_string();
            const string_t &name = object.at(u("name")).as_string();
            const string_t &status = object.at(u("status")).as_string();

            languages.emplace_back(code, name, status);
        }

        return languages;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return std::vector<language>();
}

string_t twitter_client::get_help_privacy() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::object root =
            api.request(web::http::methods::GET, u("help/privacy.json"))
                .get()
                .extract_json()
                .get()
                .as_object();

        return root.at(u("privacy")).as_string();
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return u("");
}

string_t twitter_client::get_help_tos() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::object root =
            api.request(web::http::methods::GET, u("help/tos.json"))
                .get()
                .extract_json()
                .get()
                .as_object();

        return root.at(u("tos")).as_string();
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return u("");
}
}
