include_directories(${CASABLANCA_INCLUDE_DIRS})

add_executable(test_cpprest test_cpprest.cc)

target_link_libraries(test_cpprest ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CASABLANCA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS test_cpprest RUNTIME DESTINATION bin)
