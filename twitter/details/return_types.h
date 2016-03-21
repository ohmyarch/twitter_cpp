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
enum class language : std::uint8_t {
    fr,
    en,
    ar,
    ja,
    es,
    de,
    it,
    id,
    pt,
    ko,
    tr,
    ru,
    nl,
    fil,
    msa,
    zh_tw, // zh-tw
    zh_cn, // zh-cn
    hi,
    no,
    sv,
    fi,
    da,
    pl,
    hu,
    fa,
    he,
    th,
    uk,
    cs,
    ro,
    en_gb, // en-gb
    vi,
    bn
};

enum class hour : std::int8_t {
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
    friendship(const string_t &name, const string_t &screen_name,
               std::uint64_t id, const string_t &id_str,
               const class connections &connections)
        : name_(name), screen_name_(screen_name), id_str_(id_str), id_(id),
          connections_(connections) {}
    friendship(friendship &&other) noexcept
        : name_(std::move(other.name_)),
          screen_name_(std::move(other.screen_name_)),
          id_str_(std::move(other.id_str_)),
          id_(other.id_),
          connections_(other.connections_) {}

    const string_t &name() const { return name_; }
    const string_t &screen_name() const { return screen_name_; }
    const string_t &id_str() const { return id_str_; }
    std::uint64_t id() const { return id_; }
    const class connections &connections() const { return connections_; }

  private:
    string_t name_;
    string_t screen_name_;
    string_t id_str_;
    std::uint64_t id_;
    class connections connections_;
};

class language_info {
  public:
    language_info(const string_t &code, const string_t &name,
                  const string_t &status)
        : code_(code), name_(name), status_(status) {}
    language_info(language_info &&other)
        : code_(std::move(other.code_)), name_(std::move(other.name_)),
          status_(std::move(other.status_)) {}

    const string_t &code() const { return code_; }
    const string_t &name() const { return name_; }
    const string_t &status() const { return status_; }

  private:
    string_t code_;
    string_t name_;
    string_t status_;
};

class time_zone {
  public:
    // time_zone(const string_t &name, const int utc_offset,
    // const string_t &tzinfo_name)
    // : utc_offset_(utc_offset), name_(name), tzinfo_name_(tzinfo_name) {}
    time_zone(time_zone &&other) noexcept
        : utc_offset_(other.utc_offset_),
          name_(std::move(other.name_)),
          tzinfo_name_(std::move(other.tzinfo_name_)) {}
    // time_zone(string_t &&name, const int utc_offset, string_t &&tzinfo_name)
    // : utc_offset_(utc_offset), name_(std::move(name)),
    // tzinfo_name_(std::move(tzinfo_name)) {}

    // void set_utc_offset(const int utc_offset) { utc_offset_ = utc_offset; }
    // void set_name(const string_t &name) { name_ = name; }
    // void set_tzinfo_name(const string_t &tzinfo_name) {
    // tzinfo_name_ = tzinfo_name;
    // }

    int utc_offset() const { return utc_offset_; }
    const string_t &name() const { return name_; }
    const string_t &tzinfo_name() const { return tzinfo_name_; }

  private:
    friend class twitter_client;
    friend class account_settings;

    time_zone() {}

    int utc_offset_;
    string_t name_;
    string_t tzinfo_name_;
};

class sleep_time {
  public:
    // sleep_time(const hour start_time, const hour end_time)
    //  : enabled_(true), start_time_(start_time), end_time_(end_time) {}

    bool is_enabled() const { return enabled_; }
    hour start_time() const { return start_time_; }
    hour end_time() const { return end_time_; }

  private:
    friend class twitter_client;
    friend class account_settings;

    sleep_time() {}

    bool enabled_;
    hour start_time_;
    hour end_time_;
};

class account_settings {
  public:
    account_settings(account_settings &&other) noexcept
        : protected_(other.protected_),
          geo_enabled_(other.geo_enabled_),
          always_use_https_(other.always_use_https_),
          discoverable_by_email_(other.discoverable_by_email_),
          discoverable_by_mobile_phone_(other.discoverable_by_mobile_phone_),
          use_cookie_personalization_(other.use_cookie_personalization_),
          display_sensitive_media_(other.display_sensitive_media_),
          smart_mute_(other.smart_mute_),
          allow_contributor_request_(other.allow_contributor_request_),
          allow_dms_from_(other.allow_dms_from_),
          allow_dm_groups_from_(other.allow_dm_groups_from_),
          language_(other.language_),
          screen_name_(std::move(other.screen_name_)),
          time_zone_(std::move(other.time_zone_)),
          sleep_time_(other.sleep_time_) {}

    //     void set_protected(const bool is_protected) { protected_ =
    //     is_protected; }
    //     void set_geo_enabled(const bool is_geo_enabled) {
    //         geo_enabled_ = is_geo_enabled;
    //     }
    //     void set_always_use_https(const bool is_always_use_https) {
    //         always_use_https_ = is_always_use_https;
    //     }
    //     void set_discoverable_by_email(const bool is_discoverable_by_email) {
    //         discoverable_by_email_ = is_discoverable_by_email;
    //     }
    //     void set_discoverable_by_mobile_phone(
    //         const bool is_discoverable_by_mobile_phone) {
    //         discoverable_by_mobile_phone_ = is_discoverable_by_mobile_phone;
    //     }
    //     void
    //     set_use_cookie_personalization(const bool
    //     is_use_cookie_personalization) {
    //         use_cookie_personalization_ = is_use_cookie_personalization;
    //     }
    //     void set_display_sensitive_media(const bool
    //     is_display_sensitive_media) {
    //         display_sensitive_media_ = is_display_sensitive_media;
    //     }
    //     void set_smart_mute(const bool is_smart_mute) {
    //         smart_mute_ = is_smart_mute;
    //     }
    //     void set_allow_contributor_request(const allowed who) {
    //         allow_contributor_request_ = who;
    //     }
    //     void set_allow_dms_from(const allowed who) { allow_dms_from_ = who; }
    //     void set_allow_dm_groups_from(const allowed who) {
    //         allow_dm_groups_from_ = who;
    //     }
    //     void set_time_zone(const time_zone &time_zone) { time_zone_ =
    //     time_zone; }
    //     void set_sleep_time(const sleep_time sleep_time) {
    //         sleep_time_ = sleep_time;
    //     }
    //     void set_screen_name(const string_t &screen_name) {
    //         screen_name_ = screen_name;
    //     }
    //     void set_language(const string_t &language) { language_ = language; }

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
    enum language language() const { return language_; }
    const string_t &screen_name() const { return screen_name_; }
    const class time_zone &time_zone() const { return time_zone_; }
    const class sleep_time &sleep_time() const { return sleep_time_; }

  private:
    friend class twitter_client;

    account_settings() {}

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
    enum language language_;
    string_t screen_name_;
    class time_zone time_zone_;
    class sleep_time sleep_time_;

    // bool protected_changed_;
};

class suggested_category {
  public:
    suggested_category(const string_t &name, const string_t &slug,
                       const std::uint16_t size)
        : size_(size), name_(name), slug_(slug) {}
    suggested_category(suggested_category &&other)
        : size_(other.size_), name_(std::move(other.name_)),
          slug_(std::move(other.slug_)) {}

    string_t name() const { return name_; }
    string_t slug() const { return slug_; }
    std::uint16_t size() const { return size_; }

  private:
    std::uint16_t size_;
    string_t name_;
    string_t slug_;
};

class photo_size {
  public:
    std::uint16_t h() const { return h_; }
    std::uint16_t w() const { return w_; }
    enum resize resize() const { return resize_; }

  private:
    friend class twitter_client;

    std::uint16_t h_;
    std::uint16_t w_;
    enum resize resize_;
};

class configuration {
  public:
    configuration(configuration &&other) noexcept
        : dm_text_character_limit_(other.dm_text_character_limit_),
          characters_reserved_per_media_(other.characters_reserved_per_media_),
          max_media_per_upload_(other.max_media_per_upload_),
          short_url_length_(other.short_url_length_),
          short_url_length_https_(other.short_url_length_https_),
          photo_size_limit_(other.photo_size_limit_),
          thumb_photo_size_(other.thumb_photo_size_),
          small_photo_size_(other.small_photo_size_),
          medium_photo_size_(other.medium_photo_size_),
          large_photo_size_(other.large_photo_size_),
          non_username_paths_(std::move(other.non_username_paths_)) {}

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
    const photo_size &thumb_photo_size() const { return thumb_photo_size_; }
    const photo_size &small_photo_size() const { return small_photo_size_; }
    const photo_size &medium_photo_size() const { return medium_photo_size_; }
    const photo_size &large_photo_size() const { return large_photo_size_; }
    const std::vector<std::string> &non_username_paths() const {
        return non_username_paths_;
    }

  private:
    friend class twitter_client;

    configuration() {}

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
    std::vector<string_t> non_username_paths_;
};
}
