#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "nanobench.h"
#include "tb/disjoint/algorithm.h"

TEST_CASE("foo") { REQUIRE(tb::foo() == 42); }
