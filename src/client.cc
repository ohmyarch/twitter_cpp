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

namespace twitter {
twitter_client::twitter_client(const string_t &consumer_key,
                               const string_t &consumer_secret)
    : oauth1_config_(
          consumer_key, consumer_secret,
          U("https://api.twitter.com/oauth/request_token"),
          U("https://api.twitter.com/oauth/authorize"),
          U("https://api.twitter.com/oauth/access_token"),
          U("http://testhost.local:8890/"),
          web::http::oauth1::experimental::oauth1_methods::hmac_sha1) {}

bool twitter_client::open_browser_auth() {
    if (!oauth1_config_.token().is_valid_access_token()) {
        auto auth_uri_task(oauth1_config_.build_authorization_uri());
        try {
            string_t auth_uri = auth_uri_task.get();
        } catch (const web::http::oauth1::experimental::oauth1_exception &e) {
        }

        if (true) {
            http_client_config_.set_oauth1(oauth1_config_);

            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
}
