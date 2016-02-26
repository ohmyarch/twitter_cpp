add_executable(get_account_settings get_account_settings.cc)
add_executable(get_help_languages get_help_languages.cc)

set(TWITTER_CPP_LIBRARYS
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${TWITTER_CPP_LIBRARY}
  ${CASABLANCA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(get_account_settings ${TWITTER_CPP_LIBRARYS})
target_link_libraries(get_help_languages ${TWITTER_CPP_LIBRARYS})
