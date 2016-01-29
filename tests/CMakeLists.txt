add_executable(twitter_cpp_test twitter_cpp_tests.cc)

target_link_libraries(twitter_cpp_test
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${TWITTER_CPP_LIBRARY}
  ${CASABLANCA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  )
