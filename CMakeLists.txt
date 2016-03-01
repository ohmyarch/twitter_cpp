cmake_minimum_required(VERSION 2.8.12)

project(twitter_cpp)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

option(BUILD_EXAMPLES "Build examples." ON)
option(BUILD_SHARED_LIBS "Build shared Libraries." ON)

if(WIN32)
  option(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Threads REQUIRED)
find_package(Casablanca REQUIRED)

if(BUILD_EXAMPLES)
  find_package(Boost REQUIRED COMPONENTS random chrono system thread program_options)
else()
  find_package(Boost REQUIRED COMPONENTS random chrono system thread)
endif()

if(APPLE AND NOT OPENSSL_ROOT_DIR)
  file(GLOB OPENSSL_ROOT_DIR /usr/local/Cellar/openssl/*)
  list(REVERSE OPENSSL_ROOT_DIR)
endif()
set(_OPENSSL_VERSION "")
find_package(OpenSSL 1.0.0 REQUIRED)

file(GLOB_RECURSE TWITTER_CPP_HEADERS *.h)

add_custom_target(headers SOURCES ${TWITTER_CPP_HEADERS})

include_directories(${CMAKE_SOURCE_DIR} ${OPENSSL_INCLUDE_DIR} ${CASABLANCA_INCLUDE_DIRS})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message("-- Setting clang options")

  set(WARNINGS "${WARNINGS} ${OSX_SUPPRESSIONS}")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wno-return-type-c-linkage -Wno-unneeded-internal-declaration")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-strict-aliasing")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(TWITTER_CPP_LIBRARY ${LIB}twitter)

add_subdirectory(src)

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
