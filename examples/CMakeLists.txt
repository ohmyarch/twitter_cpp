add_executable(get_account_settings get_account_settings.cc)

target_link_libraries(get_account_settings
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${TWITTER_CPP_LIBRARY}
  ${CASABLANCA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  )
