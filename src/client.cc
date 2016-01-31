#include "twitter/client.h"

namespace twitter {
twitter_client::twitter_client(const std::string &consumer_key,
                               const std::string &consumer_secret)
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
            std::string auth_uri = auth_uri_task.get();
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
