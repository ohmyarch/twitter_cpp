add_executable(get_account_settings common.cc get_account_settings.cc)
add_executable(get_help_configuration common.cc get_help_configuration.cc)
add_executable(get_help_languages common.cc get_help_languages.cc)
add_executable(get_help_privacy common.cc get_help_privacy.cc)
add_executable(get_help_tos common.cc get_help_tos.cc)

set(TWITTER_CPP_LIBRARYS
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${TWITTER_CPP_LIBRARY}
  ${CASABLANCA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(get_account_settings ${TWITTER_CPP_LIBRARYS})
target_link_libraries(get_help_configuration ${TWITTER_CPP_LIBRARYS})
target_link_libraries(get_help_languages ${TWITTER_CPP_LIBRARYS})
target_link_libraries(get_help_privacy ${TWITTER_CPP_LIBRARYS})
target_link_libraries(get_help_tos ${TWITTER_CPP_LIBRARYS})
