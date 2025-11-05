#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Mathematics/FastSphericalPolygon.h"
#include "Mathematics/SphericalPolygon.h"
#include "Mathematics/Vector3.h"
#include "Mathematics/PointS2.h"

namespace py = pybind11;

void bind_fast_sphericalpolygon(py::module& m)
{
    using Real = double;
    using FP   = gte::FastSphericalPolygon<Real>;
    using SP   = gte::SphericalPolygon<Real>;
    using Vec3 = gte::Vector3<Real>;
    using P2   = gte::PointS2<Real>;

    py::class_<FP>(m, "FastSphericalPolygond")
        // Constructors
        .def(py::init<std::vector<Vec3>, Vec3>(),
             py::arg("vertices_cartesian"), py::arg("interior_hint"))
        .def(py::init<const std::vector<P2>&, P2>(),
             py::arg("vertices_geographic"), py::arg("interior_hint"))

        // Crossing count API
        .def("NumCrossings",
             static_cast<int (FP::*)(const P2&) const>(&FP::NumCrossings),
             py::arg("query_s2"))
        .def("NumCrossings",
             static_cast<int (FP::*)(const Vec3&) const>(&FP::NumCrossings),
             py::arg("query_cart"))
        .def("NumCrossingsBatchVec3",
             static_cast<std::vector<int> (FP::*)(const std::vector<Vec3>&) const>(&FP::NumCrossings),
             py::arg("queries_cart"))
        .def("NumCrossingsBatchS2",
             static_cast<std::vector<int> (FP::*)(const std::vector<P2>&) const>(&FP::NumCrossings),
             py::arg("queries_s2"))

        // Containment API (parity)
        .def("Contains",
             static_cast<int (FP::*)(const P2&) const>(&FP::Contains),
             py::arg("query_s2"))
        .def("Contains",
             static_cast<int (FP::*)(const Vec3&) const>(&FP::Contains),
             py::arg("query_cart"))
        .def("ContainsBatchVec3",
             static_cast<std::vector<int> (FP::*)(const std::vector<Vec3>&) const>(&FP::Contains),
             py::arg("queries_cart"))
        .def("ContainsBatchS2",
             static_cast<std::vector<int> (FP::*)(const std::vector<P2>&) const>(&FP::Contains),
             py::arg("queries_s2"))

        // Access to the underlying non-preprocessed polygon
        .def("Base", &FP::Base, py::return_value_policy::reference_internal);
}