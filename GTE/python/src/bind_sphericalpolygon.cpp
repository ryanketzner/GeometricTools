#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Mathematics/SphericalPolygon.h"
#include "Mathematics/Vector3.h"
#include "Mathematics/PointS2.h"
#include "Mathematics/Matrix.h"

namespace py = pybind11;

void bind_sphericalpolygon(py::module& m)
{
    using Real = double;
    using SP   = gte::SphericalPolygon<Real>;
    using Vec3 = gte::Vector3<Real>;
    using P2   = gte::PointS2<Real>;
    using Mat3 = gte::Matrix<3,3,Real>;

    py::class_<SP>(m, "SphericalPolygond")
        // Constructors
        .def(py::init<std::vector<Vec3>, Vec3>(),
             py::arg("vertices_cartesian"), py::arg("interior_hint"))
        .def(py::init<const std::vector<P2>&, P2>(),
             py::arg("vertices_geographic"), py::arg("interior_hint"))

        // Crossing count API
        .def("NumCrossings",
             static_cast<int (SP::*)(const P2&) const>(&SP::NumCrossings),
             py::arg("query_s2"))
        .def("NumCrossings",
             static_cast<int (SP::*)(const Vec3&) const>(&SP::NumCrossings),
             py::arg("query_cart"))
        .def("NumCrossingsBatchVec3",
             static_cast<std::vector<int> (SP::*)(const std::vector<Vec3>&) const>(&SP::NumCrossings),
             py::arg("queries_cart"))
        .def("NumCrossingsBatchS2",
             static_cast<std::vector<int> (SP::*)(const std::vector<P2>&) const>(&SP::NumCrossings),
             py::arg("queries_s2"))

        // Containment API (parity)
        .def("Contains",
             static_cast<int (SP::*)(const P2&) const>(&SP::Contains),
             py::arg("query_s2"))
        .def("Contains",
             static_cast<int (SP::*)(const Vec3&) const>(&SP::Contains),
             py::arg("query_cart"))
        .def("ContainsBatchVec3",
             static_cast<std::vector<int> (SP::*)(const std::vector<Vec3>&) const>(&SP::Contains),
             py::arg("queries_cart"))
        .def("ContainsBatchS2",
             static_cast<std::vector<int> (SP::*)(const std::vector<P2>&) const>(&SP::Contains),
             py::arg("queries_s2"))
        .def("SetTransform",
         [](SP& self, const Mat3& IN){ self.SetTransform(IN); },
         py::arg("IN"))


        // Accessors
        .def("GetLonArrayQ", &SP::GetLonArrayQ)
        .def("QI", &SP::QI, py::return_value_policy::reference_internal);
}