include_directories(${CASABLANCA_INCLUDE_DIRS})

add_executable(twitter_cpp_test twitter_cpp_tests.cc)

target_link_libraries(twitter_cpp_test ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CASABLANCA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
