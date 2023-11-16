cmake_minimum_required(VERSION 3.27)
project(
  disjoint
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(disjoint src/algorithm.cc)
target_include_directories(disjoint PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_options(
  disjoint
  PRIVATE -Wall
          -Wextra
          -Wshadow
          -Wunused
          -Wold-style-cast
          -Wcast-align
          -Wpedantic
          -Wconversion
          -fno-exceptions
          $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=address>
          $<$<CONFIG:Debug,RelWithDebInfo>:-fno-omit-frame-pointer>)
target_link_options(disjoint PUBLIC
                    $<$<CONFIG:Debug,RelWithDebInfo>:-fsanitize=address>)

include(${CMAKE_CURRENT_LIST_DIR}/tests/Tests.cmake)
