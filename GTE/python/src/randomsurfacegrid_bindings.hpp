#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include "Mathematics/Hypersphere.h"
#include "Mathematics/Hyperellipsoid.h"
#include "Mathematics/GridHyperellipsoid.h"
#include "Mathematics/GridHypersphere.h"


namespace py = pybind11;
using namespace gte;

template<int N, typename Real>
void bind_random_surface_grid(py::module& m, const std::string& func_name) {
    m.def(func_name.c_str(), py::overload_cast<int, Hypersphere<N, Real> const&, unsigned int>(&RandomSurfaceGrid<N, Real>), py::arg("num"), py::arg("sphere"), py::arg("seed") = std::random_device{}());
    m.def(func_name.c_str(), py::overload_cast<int, Hyperellipsoid<N, Real> const&, unsigned int>(&RandomSurfaceGrid<N, Real>), py::arg("num"), py::arg("ellipsoid"), py::arg("seed") = std::random_device{}());
}