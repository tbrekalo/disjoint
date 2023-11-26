set(DISJOINT_TEST_FETCH_TARGETS "")

find_package(doctest 2.4 QUIET)
if(NOT doctest_FOUND)
  FetchContent_Declare(
    doctest
    GIT_REPOSITORY git@github.com:doctest/doctest.git
    GIT_TAG v2.4.11
    GIT_SHALLOW TRUE)

  list(APPEND DISJOINT_TEST_FETCH_TARGETS doctest)
endif()

find_package(nanobench 4.1 QUIET)
if(NOT nanobench_FOUND)
  FetchContent_Declare(
    nanobench
    GIT_REPOSITORY https://github.com/martinus/nanobench.git
    GIT_TAG v4.1.0
    GIT_SHALLOW TRUE)

  list(APPEND DISJOINT_TEST_FETCH_TARGETS nanobench)
endif()

FetchContent_MakeAvailable(${DISJOINT_TEST_FETCH_TARGETS})
add_executable(disjoint-test ${CMAKE_CURRENT_LIST_DIR}/src/main.cc)
target_link_libraries(disjoint-test PRIVATE disjoint doctest::doctest nanobench)
