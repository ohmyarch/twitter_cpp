include_directories(${CASABLANCA_INCLUDE_DIRS})

add_executable(test test.cc)

target_link_libraries(test ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CASABLANCA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
