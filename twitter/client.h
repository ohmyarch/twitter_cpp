#pragma once

#include <cpprest/http_client.h>
#include <cpprest/oauth1.h>
#include <string>

namespace twitter {
enum class error : std::uint8_t {};

class twitter_client {
  public:
    twitter_client(const std::string &consumer_key,
                   const std::string &consumer_secret);

    bool open_browser_auth();
    bool pin_auth();

  protected:
    web::http::client::http_client_config http_client_config_;
    web::http::oauth1::experimental::oauth1_config oauth1_config_;
};
}
