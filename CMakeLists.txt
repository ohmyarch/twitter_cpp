cmake_minimum_required(VERSION 2.8.12)

project(twitter_cpp)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

option(BUILD_EXAMPLES "Build examples." ON)

find_package(Threads REQUIRED)
find_package(Casablanca REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

if(APPLE AND NOT OPENSSL_ROOT_DIR)
  file(GLOB OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/*)
  list(REVERSE OPENSSL_ROOT_DIR)
endif()
  set(_OPENSSL_VERSION "")
find_package(OpenSSL 1.0.0 REQUIRED)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message("-- Setting clang options")
  
  set(WARNINGS "${WARNINGS} ${OSX_SUPPRESSIONS}")
  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wno-return-type-c-linkage -Wno-unneeded-internal-declaration")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-strict-aliasing")
endif()

add_subdirectory(src)

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
