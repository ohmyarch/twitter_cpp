/****************************************************************************
 * *
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
#include <vector>

namespace twitter {
enum class hour : std::int8_t {
    null = -1,
    _00 = 0,
    _01 = 1,
    _02 = 2,
    _03 = 3,
    _04 = 4,
    _05 = 5,
    _06 = 6,
    _07 = 7,
    _08 = 8,
    _09 = 9,
    _10 = 10,
    _11 = 11,
    _12 = 12,
    _13 = 13,
    _14 = 14,
    _15 = 15,
    _16 = 16,
    _17 = 17,
    _18 = 18,
    _19 = 19,
    _20 = 20,
    _21 = 21,
    _22 = 22,
    _23 = 23
};

enum class allowed : std::uint8_t { all, following, none };

enum class resize : std::uint8_t { fit, crop };

class connections {
  public:
    bool is_following() const { return following_; }
    bool is_following_requested() const { return following_requested_; }
    bool is_followed_by() const { return followed_by_; }
    bool is_none() const { return none_; }
    bool is_blocking() const { return blocking_; }
    bool is_muting() const { return muting_; }

  private:
    friend class twitter_client;

    bool following_ = false;
    bool following_requested_ = false;
    bool followed_by_ = false;
    bool none_ = false;
    bool blocking_ = false;
    bool muting_ = false;
};

class friendship {
  public:
    string_t name() const { return name_; }
    string_t screen_name() const { return screen_name_; }
    string_t id_str() const { return id_str_; }
    std::uint64_t id() const { return id_; }
    twitter::connections connections() const { return connections_; }

  private:
    friend class twitter_client;

    string_t name_;
    string_t screen_name_;
    string_t id_str_;
    std::uint64_t id_;
    twitter::connections connections_;
};

class language {
  public:
    language(const string_t &code, const string_t &name, const string_t &status)
        : code_(code), name_(name), status_(status) {}

    void set_code(const string_t &code) { code_ = code; }
    void set_name(const string_t &name) { name_ = name; }
    void set_status(const string_t &status) { status_ = status; }

    string_t code() const { return code_; }
    string_t name() const { return name_; }
    string_t status() const { return status_; }

  private:
    string_t code_;
    string_t name_;
    string_t status_;
};

class time_zone {
  public:
    time_zone(){};
    time_zone(const string_t &name, const int utc_offset,
              const string_t &tzinfo_name)
        : utc_offset_(utc_offset), name_(name), tzinfo_name_(tzinfo_name) {}

    void set_utc_offset(const int utc_offset) { utc_offset_ = utc_offset; }
    void set_name(const string_t &name) { name_ = name; }
    void set_tzinfo_name(const string_t &tzinfo_name) {
        tzinfo_name_ = tzinfo_name;
    }

    int utc_offset() const { return utc_offset_; }
    string_t name() const { return name_; }
    string_t tzinfo_name() const { return tzinfo_name_; }

  private:
    int utc_offset_;
    string_t name_;
    string_t tzinfo_name_;
};

class sleep_time {
  public:
    sleep_time() {}
    sleep_time(const hour start_time, const hour end_time) {
        if ((start_time != hour::null) && (end_time != hour::null)) {
            start_time_ = start_time;
            end_time_ = end_time;
            enabled_ = true;
        }
    }

    bool is_enabled() const { return enabled_; }
    hour start_time() const { return start_time_; }
    hour end_time() const { return end_time_; }

  private:
    bool enabled_ = false;
    hour start_time_ = hour::null;
    hour end_time_ = hour::null;
};

class account_settings {
  public:
    account_settings(){};

    void set_protected(const bool is_protected) { protected_ = is_protected; }
    void set_geo_enabled(const bool is_geo_enabled) {
        geo_enabled_ = is_geo_enabled;
    }
    void set_always_use_https(const bool is_always_use_https) {
        always_use_https_ = is_always_use_https;
    }
    void set_discoverable_by_email(const bool is_discoverable_by_email) {
        discoverable_by_email_ = is_discoverable_by_email;
    }
    void set_discoverable_by_mobile_phone(
        const bool is_discoverable_by_mobile_phone) {
        discoverable_by_mobile_phone_ = is_discoverable_by_mobile_phone;
    }
    void
    set_use_cookie_personalization(const bool is_use_cookie_personalization) {
        use_cookie_personalization_ = is_use_cookie_personalization;
    }
    void set_display_sensitive_media(const bool is_display_sensitive_media) {
        display_sensitive_media_ = is_display_sensitive_media;
    }
    void set_smart_mute(const bool is_smart_mute) {
        smart_mute_ = is_smart_mute;
    }
    void set_allow_contributor_request(const allowed who) {
        allow_contributor_request_ = who;
    }
    void set_allow_dms_from(const allowed who) { allow_dms_from_ = who; }
    void set_allow_dm_groups_from(const allowed who) {
        allow_dm_groups_from_ = who;
    }
    void set_time_zone(const time_zone &time_zone) { time_zone_ = time_zone; }
    void set_sleep_time(const sleep_time sleep_time) {
        sleep_time_ = sleep_time;
    }
    void set_screen_name(const string_t &screen_name) {
        screen_name_ = screen_name;
    }
    void set_language(const string_t &language) { language_ = language; }

    bool is_protected() const { return protected_; }
    bool is_geo_enabled() const { return geo_enabled_; }
    bool is_always_use_https() const { return always_use_https_; }
    bool is_discoverable_by_email() const { return discoverable_by_email_; }
    bool is_discoverable_by_mobile_phone() const {
        return discoverable_by_mobile_phone_;
    }
    bool is_use_cookie_personalization() const {
        return use_cookie_personalization_;
    }
    bool is_display_sensitive_media() const { return display_sensitive_media_; }
    bool smart_mute() const { return smart_mute_; }
    allowed allow_contributor_request() const {
        return allow_contributor_request_;
    }
    allowed allow_dms_from() const { return allow_dms_from_; }
    allowed allow_dm_groups_from() const { return allow_dm_groups_from_; }
    twitter::time_zone time_zone() const { return time_zone_; }
    twitter::sleep_time sleep_time() const { return sleep_time_; }
    string_t screen_name() const { return screen_name_; }
    string_t language() const { return language_; }

  private:
    bool protected_;
    bool geo_enabled_;
    bool always_use_https_;
    bool discoverable_by_email_;
    bool discoverable_by_mobile_phone_;
    bool use_cookie_personalization_;
    bool display_sensitive_media_;
    bool smart_mute_;
    allowed allow_contributor_request_;
    allowed allow_dms_from_;
    allowed allow_dm_groups_from_;
    twitter::time_zone time_zone_;
    twitter::sleep_time sleep_time_;
    string_t screen_name_;
    string_t language_;

    // bool protected_changed_;
};

class photo_size {
  public:
    std::uint16_t h() const { return h_; }
    std::uint16_t w() const { return w_; }
    twitter::resize resize() const { return resize_; }

  private:
    friend class twitter_client;

    std::uint16_t h_;
    std::uint16_t w_;
    twitter::resize resize_;
};

class configuration {
  public:
    std::uint16_t dm_text_character_limit() const {
        return dm_text_character_limit_;
    }
    std::uint16_t characters_reserved_per_media() const {
        return characters_reserved_per_media_;
    }
    std::uint16_t max_media_per_upload() const { return max_media_per_upload_; }
    std::uint16_t short_url_length() const { return short_url_length_; }
    std::uint16_t short_url_length_https() const {
        return short_url_length_https_;
    }
    std::uint32_t photo_size_limit() const { return photo_size_limit_; }
    photo_size thumb_photo_size() const { return thumb_photo_size_; }
    photo_size small_photo_size() const { return small_photo_size_; }
    photo_size medium_photo_size() const { return medium_photo_size_; }
    photo_size large_photo_size() const { return large_photo_size_; }
    std::vector<std::string> non_username_paths() const {
        return non_username_paths_;
    }

  private:
    friend class twitter_client;

    std::uint16_t dm_text_character_limit_;
    std::uint16_t characters_reserved_per_media_;
    std::uint16_t max_media_per_upload_;
    std::uint16_t short_url_length_;
    std::uint16_t short_url_length_https_;
    std::uint32_t photo_size_limit_;
    photo_size thumb_photo_size_;
    photo_size small_photo_size_;
    photo_size medium_photo_size_;
    photo_size large_photo_size_;
    std::vector<twitter::string_t> non_username_paths_;
};
}
