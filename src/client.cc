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
#include <boost/optional.hpp>
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

    return {};
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

        const web::json::value root =
            api.request(web::http::methods::GET, builder.to_string())
                .get()
                .extract_json()
                .get();

        std::vector<friendship> friendships;

        for (const auto &object : root.as_array()) {
            const string_t &name = object.at(u("name")).as_string();
            const string_t &screen_name =
                object.at(u("screen_name")).as_string();
            const std::uint64_t &id =
                object.at(u("id")).as_number().to_uint64();
            const string_t &id_str = object.at(u("id_str")).as_string();

            connections connections;

            const web::json::array &connections_node =
                object.at(u("connections")).as_array();
            for (const auto &connection : connections_node) {
                const string_t &connection_type = connection.as_string();
                if (connection_type == u("none")) {
                    connections.none_ = true;

                    break;
                } else if (connection_type == u("following")) {
                    connections.following_ = true;
                } else if (connection_type == u("following_requested")) {
                    connections.following_requested_ = true;
                } else if (connection_type == u("followed_by")) {
                    connections.followed_by_ = true;
                } else if (connection_type == u("blocking")) {
                    connections.blocking_ = true;
                } else if (connection_type == u("muting")) {
                    connections.muting_ = true;
                }
            }

            friendships.emplace_back(name, screen_name, id, id_str,
                                     connections);
        }

        return friendships;
    } catch (const web::uri_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return {};
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

        const web::json::value root =
            api.request(web::http::methods::GET, builder.to_string())
                .get()
                .extract_json()
                .get();

        std::vector<friendship> friendships;

        for (const auto &object : root.as_array()) {
            const string_t &name = object.at(u("name")).as_string();
            const string_t &screen_name =
                object.at(u("screen_name")).as_string();
            const std::uint64_t &id =
                object.at(u("id")).as_number().to_uint64();
            const string_t &id_str = object.at(u("id_str")).as_string();

            connections connections;

            const web::json::array &connections_node =
                object.at(u("connections")).as_array();
            for (const auto &connection : connections_node) {
                const string_t &connection_type = connection.as_string();
                if (connection_type == u("none")) {
                    connections.none_ = true;

                    break;
                } else if (connection_type == u("following")) {
                    connections.following_ = true;
                } else if (connection_type == u("following_requested")) {
                    connections.following_requested_ = true;
                } else if (connection_type == u("followed_by")) {
                    connections.followed_by_ = true;
                } else if (connection_type == u("blocking")) {
                    connections.blocking_ = true;
                } else if (connection_type == u("muting")) {
                    connections.muting_ = true;
                }
            }

            friendships.emplace_back(name, screen_name, id, id_str,
                                     connections);
        }

        return friendships;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return {};
}

std::experimental::optional<account_settings>
twitter_client::get_account_settings() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::value root =
            api.request(web::http::methods::GET, u("account/settings.json"))
                .get()
                .extract_json()
                .get();

        account_settings settings;

        const web::json::value &time_zone = root.at(u("time_zone"));
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

        const web::json::value &sleep_time = root.at(u("sleep_time"));
        if ((settings.sleep_time_.enabled_ =
                 sleep_time.at(u("enabled")).as_bool())) {
            settings.sleep_time_.start_time_ =
                static_cast<hour>(sleep_time.at(u("start_time")).as_integer());
            settings.sleep_time_.end_time_ =
                static_cast<hour>(sleep_time.at(u("end_time")).as_integer());
        }

        settings.geo_enabled_ = root.at(u("geo_enabled")).as_bool();

        auto string_to_language = [](const string_t &str) {
            if (str == "fr")
                return language::fr;
            if (str == "en")
                return language::en;
            if (str == "ar")
                return language::ar;
            if (str == "ja")
                return language::ja;
            if (str == "es")
                return language::es;
            if (str == "de")
                return language::de;
            if (str == "it")
                return language::it;
            if (str == "id")
                return language::id;
            if (str == "pt")
                return language::pt;
            if (str == "ko")
                return language::ko;
            if (str == "tr")
                return language::tr;
            if (str == "ru")
                return language::ru;
            if (str == "nl")
                return language::nl;
            if (str == "fil")
                return language::fil;
            if (str == "msa")
                return language::msa;
            if (str == "zh-TW")
                return language::zh_tw;
            if (str == "zh-CN")
                return language::zh_cn;
            if (str == "hi")
                return language::hi;
            if (str == "no")
                return language::no;
            if (str == "sv")
                return language::sv;
            if (str == "fi")
                return language::fi;
            if (str == "da")
                return language::da;
            if (str == "pl")
                return language::pl;
            if (str == "hu")
                return language::hu;
            if (str == "fa")
                return language::fa;
            if (str == "he")
                return language::he;
            if (str == "th")
                return language::th;
            if (str == "uk")
                return language::uk;
            if (str == "cs")
                return language::cs;
            if (str == "ro")
                return language::ro;
            if (str == "en_GB")
                return language::en_gb;
            if (str == "vi")
                return language::vi;

            return language::bn;
        };
        settings.language_ =
            string_to_language(root.at(u("language")).as_string());

        settings.discoverable_by_email_ =
            root.at(u("discoverable_by_email")).as_bool();
        settings.discoverable_by_mobile_phone_ =
            root.at(u("discoverable_by_mobile_phone")).as_bool();
        settings.display_sensitive_media_ =
            root.at(u("display_sensitive_media")).as_bool();
        settings.smart_mute_ = root.at(u("smart_mute")).as_bool();

        auto string_to_allowed = [](const string_t &str) {
            if (str == u("all"))
                return allowed::all;
            if (str == u("following"))
                return allowed::following;

            return allowed::none;
        };
        settings.allow_contributor_request_ = string_to_allowed(
            root.at(u("allow_contributor_request")).as_string());
        settings.allow_dms_from_ =
            string_to_allowed(root.at(u("allow_dms_from")).as_string());
        settings.allow_dm_groups_from_ =
            string_to_allowed(root.at(u("allow_dm_groups_from")).as_string());

        return settings;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return {};
}

std::vector<user> twitter_client::get_users_lookup(
    const std::vector<string_t> &screen_names) const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    string_t query;
    for (const auto &e : screen_names)
        query += (e + web::uri::encode_data_string(u(",")));

    query.resize(query.size() - 3);

    try {
        web::uri_builder builder(u("users/lookup.json"));
        builder.append_query(u("screen_name"), query, false);

        const web::json::value root =
            api.request(web::http::methods::GET, builder.to_string())
                .get()
                .extract_json()
                .get();

        std::vector<user> users;

        for (const auto &object : root.as_array()) {
            user user;
            user.name_ = object.at(u("name")).as_string();
            user.profile_sidebar_fill_color_ =
                object.at(u("profile_sidebar_fill_color")).as_string();
            user.profile_background_tile_ =
                object.at(u("profile_background_tile")).as_bool();
            user.profile_sidebar_border_color_ =
                object.at(u("profile_sidebar_border_color")).as_string();
            user.profile_image_url_ =
                object.at(u("profile_image_url")).as_string();
            user.location_ = object.at(u("location")).as_string();

            std::vector<string_t> created_at(6);
            boost::split(created_at, object.at(u("created_at")).as_string(),
                         boost::is_any_of(u(" ")));
            const std::uint16_t year =
                static_cast<std::uint16_t>(std::stoi(created_at[5]));

            std::uint16_t month;
            const string_t &month_str = created_at[1];
            if (month_str == "Jan")
                month = 1;
            else if (month_str == "Feb")
                month = 2;
            else if (month_str == "Mar")
                month = 3;
            else if (month_str == "Apr")
                month = 4;
            else if (month_str == "May")
                month = 5;
            else if (month_str == "Jun")
                month = 6;
            else if (month_str == "Jul")
                month = 7;
            else if (month_str == "Aug")
                month = 8;
            else if (month_str == "Sep")
                month = 9;
            else if (month_str == "Oct")
                month = 10;
            else if (month_str == "Nov")
                month = 11;
            else
                month = 12;

            const std::uint16_t day =
                static_cast<std::uint16_t>(std::stoi(created_at[2]));

            std::vector<string_t> time(3);
            boost::split(time, created_at[3], boost::is_any_of(u(":")));
            const std::uint16_t hour =
                static_cast<std::uint16_t>(std::stoi(time[0]));
            const std::uint16_t minute =
                static_cast<std::uint16_t>(std::stoi(time[1]));
            const std::uint16_t second =
                static_cast<std::uint16_t>(std::stoi(time[2]));

            const std::int32_t offset =
                std::stoi(created_at[4].substr(1, 2)) * 3600;

            user.created_at_ =
                date_time(year, month, day, hour, minute, second, offset);

            const web::json::value &follow_request_sent =
                object.at(u("follow_request_sent"));
            if (!follow_request_sent.is_null())
                user.follow_request_sent_ = follow_request_sent.as_bool();

            user.id_str_ = object.at(u("id_str")).as_string();
            user.profile_link_color_ =
                object.at(u("profile_link_color")).as_string();
            user.is_translator_ = object.at(u("is_translator")).as_bool();
            user.default_profile_ = object.at(u("default_profile")).as_bool();
            user.favourites_count_ =
                object.at(u("favourites_count")).as_number().to_uint32();
            user.contributors_enabled_ =
                object.at(u("contributors_enabled")).as_bool();

            const web::json::value &url = object.at(u("url"));
            if (!url.is_null())
                user.url_ = url.as_string();

            user.profile_image_url_https_ =
                object.at(u("profile_image_url_https")).as_string();

            const web::json::value &utc_offset = object.at(u("utc_offset"));
            if (!utc_offset.is_null())
                user.utc_offset_ = utc_offset.as_integer();

            user.id_ = object.at(u("id")).as_number().to_uint64();
            user.profile_use_background_image_ =
                object.at(u("profile_use_background_image")).as_bool();
            user.listed_count_ =
                object.at(u("listed_count")).as_number().to_uint32();

            auto string_to_language = [](const string_t &str) {
                if (str == "fr")
                    return language::fr;
                if (str == "en")
                    return language::en;
                if (str == "ar")
                    return language::ar;
                if (str == "ja")
                    return language::ja;
                if (str == "es")
                    return language::es;
                if (str == "de")
                    return language::de;
                if (str == "it")
                    return language::it;
                if (str == "id")
                    return language::id;
                if (str == "pt")
                    return language::pt;
                if (str == "ko")
                    return language::ko;
                if (str == "tr")
                    return language::tr;
                if (str == "ru")
                    return language::ru;
                if (str == "nl")
                    return language::nl;
                if (str == "fil")
                    return language::fil;
                if (str == "msa")
                    return language::msa;
                if (str == "zh-TW")
                    return language::zh_tw;
                if (str == "zh-CN")
                    return language::zh_cn;
                if (str == "hi")
                    return language::hi;
                if (str == "no")
                    return language::no;
                if (str == "sv")
                    return language::sv;
                if (str == "fi")
                    return language::fi;
                if (str == "da")
                    return language::da;
                if (str == "pl")
                    return language::pl;
                if (str == "hu")
                    return language::hu;
                if (str == "fa")
                    return language::fa;
                if (str == "he")
                    return language::he;
                if (str == "th")
                    return language::th;
                if (str == "uk")
                    return language::uk;
                if (str == "cs")
                    return language::cs;
                if (str == "ro")
                    return language::ro;
                if (str == "en_GB")
                    return language::en_gb;
                if (str == "vi")
                    return language::vi;

                return language::bn;
            };
            user.lang_ = string_to_language(object.at(u("lang")).as_string());

            user.followers_count_ =
                object.at(u("followers_count")).as_number().to_uint64();
            user.protected_ = object.at(u("protected")).as_bool();
            user.profile_background_image_url_https_ =
                object.at(u("profile_background_image_url_https")).as_bool();
            user.geo_enabled_ = object.at(u("geo_enabled")).as_bool();

            const web::json::value &description = object.at(u("description"));
            if (!description.is_null())
                user.description_ = description.as_string();

            user.profile_background_color_ =
                object.at(u("profile_background_color")).as_string();
            user.verified_ = object.at(u("verified")).as_bool();

            const web::json::value &time_zone = object.at(u("time_zone"));
            if (!time_zone.is_null())
                user.time_zone_ = time_zone.as_string();

            user.statuses_count_ =
                object.at(u("statuses_count")).as_number().to_uint32();
            user.profile_background_image_url_ =
                object.at(u("profile_background_image_url")).as_string();
            user.default_profile_image_ =
                object.at(u("default_profile_image")).as_bool();
            user.friends_count_ =
                object.at(u("friends_count")).as_number().to_uint32();
            user.screen_name_ = object.at(u("screen_name")).as_string();
            user.show_all_inline_media_ =
                object.at(u("show_all_inline_media")).as_bool();

            if (object.has_field(u("status"))) {
                const web::json::value &status = object.at(u("status"));
                if (!status.is_null()) {
                    const web::json::object &status_object = status.as_object();
                    if (!status_object.empty()) {
                        ;
                    }
                }
            }

            users.emplace_back(std::move(user));
        }

        return users;
    } catch (const web::uri_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return {};
}

std::vector<user> twitter_client::get_users_lookup(
    const std::vector<std::uint64_t> &user_ids) const {
    return {};
}

std::vector<suggested_category>
twitter_client::get_users_suggestions(const language lang) const {
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
            return u("zh-tw");
        case twitter::language::zh_cn:
            return u("zh-cn");
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
            return u("en-gb");
        case twitter::language::vi:
            return u("vi");
        case twitter::language::bn:
            return u("bn");
        }
    };

    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {

        web::uri_builder builder(u("users/suggestions.json"));
        builder.append_query(u("lang"), language_to_string(lang), false);

        const web::json::value root =
            api.request(web::http::methods::GET, builder.to_string())
                .get()
                .extract_json()
                .get();

        std::vector<suggested_category> suggested_categories;

        for (const auto &object : root.as_array()) {
            const string_t &name = object.at(u("name")).as_string();
            const string_t &slug = object.at(u("slug")).as_string();
            const std::uint16_t size =
                static_cast<std::uint16_t>(object.at(u("size")).as_integer());

            suggested_categories.emplace_back(name, slug, size);
        }

        return suggested_categories;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return {};
}

std::experimental::optional<configuration>
twitter_client::get_help_configuration() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::value root =
            api.request(web::http::methods::GET, u("help/configuration.json"))
                .get()
                .extract_json()
                .get();

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

        const web::json::value &photo_sizes = root.at(u("photo_sizes"));

        const web::json::value &thumb = photo_sizes.at(u("thumb"));
        config.thumb_photo_size_.h_ =
            static_cast<std::uint16_t>(thumb.at(u("h")).as_integer());
        config.thumb_photo_size_.w_ =
            static_cast<std::uint16_t>(thumb.at(u("w")).as_integer());
        config.thumb_photo_size_.resize_ =
            (thumb.at(u("resize")).as_string() == u("fit")) ? resize::fit
                                                            : resize::crop;

        const web::json::value &small = photo_sizes.at(u("small"));
        config.small_photo_size_.h_ =
            static_cast<std::uint16_t>(small.at(u("h")).as_integer());
        config.small_photo_size_.w_ =
            static_cast<std::uint16_t>(small.at(u("w")).as_integer());
        config.small_photo_size_.resize_ =
            (small.at(u("resize")).as_string() == u("fit")) ? resize::fit
                                                            : resize::crop;

        const web::json::value &medium = photo_sizes.at(u("medium"));
        config.medium_photo_size_.h_ =
            static_cast<std::uint16_t>(medium.at(u("h")).as_integer());
        config.medium_photo_size_.w_ =
            static_cast<std::uint16_t>(medium.at(u("w")).as_integer());
        config.medium_photo_size_.resize_ =
            (medium.at(u("resize")).as_string() == u("fit")) ? resize::fit
                                                             : resize::crop;

        const web::json::value &large = photo_sizes.at(u("large"));
        config.large_photo_size_.h_ =
            static_cast<std::uint16_t>(large.at(u("h")).as_integer());
        config.large_photo_size_.w_ =
            static_cast<std::uint16_t>(large.at(u("w")).as_integer());
        config.large_photo_size_.resize_ =
            (large.at(u("resize")).as_string() == u("fit")) ? resize::fit
                                                            : resize::crop;

        const web::json::array &non_username_paths =
            root.at(u("non_username_paths")).as_array();
        for (const auto &e : non_username_paths)
            config.non_username_paths_.emplace_back(e.as_string());

        return config;
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return {};
}

std::vector<language_info> twitter_client::get_help_languages() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::value root =
            api.request(web::http::methods::GET, u("help/languages.json"))
                .get()
                .extract_json()
                .get();

        std::vector<language_info> languages;

        for (const auto &object : root.as_array()) {
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

    return {};
}

string_t twitter_client::get_help_privacy() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::value root =
            api.request(web::http::methods::GET, u("help/privacy.json"))
                .get()
                .extract_json()
                .get();

        return root.at(u("privacy")).as_string();
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return {};
}

string_t twitter_client::get_help_tos() const {
    web::http::client::http_client api(u("https://api.twitter.com/1.1/"),
                                       http_client_config_);

    try {
        const web::json::value root =
            api.request(web::http::methods::GET, u("help/tos.json"))
                .get()
                .extract_json()
                .get();

        return root.at(u("tos")).as_string();
    } catch (const web::http::http_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (const web::json::json_exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return {};
}
}
