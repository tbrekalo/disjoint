cmake_minimum_required(VERSION 3.27)
project(
  disjoint
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(disjoint src/algorithm.cc src/subset.cc)
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

if(SKBUILD)
  message(STATUS "Python_VERSION ${PYTHON_VERSION_STRING}")
  message(STATUS "Python_EXECUTABLE ${PYTHON_EXECUTABLE}")
  message(STATUS "Python_INCLUDE_DIR ${PYTHON_INCLUDE_DIR}")
  message(STATUS "Python_LIBRARIES
${PYTHON_LIBRARY}")
  set(Python_VERSION "${PYTHON_VERSION_STRING}")
  set(Python_EXECUTABLE "${PYTHON_EXECUTABLE}")
  set(Python_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
  set(Python_LIBRARIES "${PYTHON_LIBRARY}")
endif()

find_package(Python 3.8 COMPONENTS Interpreter Development.Module)
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
# Now import nanobind from CMake
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(dispy src/bind.cc)
target_link_libraries(dispy PRIVATE disjoint)

install(TARGETS dispy LIBRARY DESTINATION .)
