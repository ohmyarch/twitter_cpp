#include <iostream>
#include <cpprest/http_client.h>

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
  web::http::client::http_client client(U("https://github.com/"));

  std::cout
      << client.request(web::http::methods::GET).get().extract_string().get()
      << std::endl;
  ;
}
