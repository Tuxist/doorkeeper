project(pidoorkeeper)
cmake_minimum_required(VERSION 2.6)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
  if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
     SET(CMAKE_CXX_FLAGS "-g -Wall -export-dynamic -std=c++11 -fPIC")
   endif( CMAKE_CXX_COMPILER_ID MATCHES GNU )
ELSE(CMAKE_BUILD_TYPE MATCHES DEBUG )
  if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
    SET(CMAKE_CXX_FLAGS "-O0 -export-dynamic -Wall -std=c++11 -fPIC")
  endif( CMAKE_CXX_COMPILER_ID MATCHES GNU )
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

if( CMAKE_CXX_COMPILER_ID MATCHES Clang )
  SET(CMAKE_CXX_FLAGS "-std=c++11")
ENDIF( CMAKE_CXX_COMPILER_ID MATCHES Clang )

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
