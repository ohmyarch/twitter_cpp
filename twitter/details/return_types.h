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
    zh_tw,
    zh_cn,
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
    en_gb,
    vi,
    bn
};

enum class allowed : std::uint8_t { all, following, none };

class language_info {
  public:
    language_info(const language lang) {
        switch (lang) {
        case language::fr:
            break;
        case language::en:
            break;
        case language::ar:
            break;
        case language::ja:
            break;
        case language::es:
            break;
        case language::de:
            break;
        case language::it:
            break;
        case language::id:
            break;
        case language::pt:
            break;
        case language::ko:
            break;
        case language::tr:
            break;
        case language::ru:
            break;
        case language::nl:
            break;
        case language::fil:
            break;
        case language::msa:
            break;
        case language::zh_tw:
            break;
        case language::zh_cn:
            break;
        case language::hi:
            break;
        case language::no:
            break;
        case language::sv:
            break;
        case language::fi:
            break;
        case language::da:
            break;
        case language::pl:
            break;
        case language::hu:
            break;
        case language::fa:
            break;
        case language::he:
            break;
        case language::th:
            break;
        case language::uk:
            break;
        case language::cs:
            break;
        case language::ro:
            break;
        case language::en_gb:
            break;
        case language::vi:
            break;
        case language::bn:
            break;
        }
    }
    language_info(const string_t &code, const string_t &name,
                  const string_t &status) {}

    void set_code(const string_t &code) { code_ = code; }
    void set_name(const string_t &name) { name_ = name; }
    void set_status(const string_t &status) { status_ = status; }

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
    sleep_time();

  private:
    bool enabled_;
    int end_time_;
    int start_time_;
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
    void set_screen_name(const string_t &screen_name) {
        screen_name_ = screen_name;
    }

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
    string_t screen_name() const { return screen_name_; }

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
    string_t screen_name_;

    // bool protected_changed_;
};
}
