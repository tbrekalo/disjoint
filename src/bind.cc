#include <ranges>

#include "nanobind/nanobind.h"
#include "nanobind/ndarray.h"
#include "nanobind/stl/bind_vector.h"
#include "tb/disjoint/algorithm.h"
#include "tb/disjoint/subset.h"

namespace nb = nanobind;

using nb_subset_array = nb::ndarray<int, nb::ndim<2>, nb::device::cpu>;

static auto create_subsets(nb_subset_array subsets) -> std::vector<tb::subset> {
  auto dst =
      std::vector<tb::subset>(subsets.shape(0), tb::subset(subsets.shape(1)));

  for (auto subset_idx : std::views::iota(0uz, subsets.shape(0))) {
    for (auto item_idx :
         std::views::iota(0uz, subsets.shape(1)) |
             std::views::filter([subsets, subset_idx](auto item_idx) -> bool {
               return subsets(subset_idx, item_idx) == 1;
             })) {
      dst[subset_idx].insert(item_idx);
    }
  }

  return dst;
}

NB_MODULE(dispy, m) {
  nb::class_<tb::subset>(m, "Subset", "A binary vector subset encoding.")
      .def(nb::init<std::size_t>())
      .def("__getitem__", &tb::subset::contains,
           "Returns true if item is in a subset.")
      .def("contains", &tb::subset::contains,
           "Returns true if item is in a subset.")
      .def("__len__", &tb::subset::size, "Returns size of a subset.")
      .def("insert", &tb::subset::insert, "Inserts item into a subset.")
      .def("erase", &tb::subset::erase, "Removes item from a subset.")
      .def("clear", &tb::subset::erase, "Clears a subset.");

  nb::bind_vector<std::vector<tb::subset>>(m, "SubsetList");

  m.def("create_subsets", &create_subsets,
        "Creates an array of dispy.subset objects from ndarray object.");

  m.def("unique", tb::unique,
        "Finds unique subsets in an array of dispy.Subset objects.");
}
