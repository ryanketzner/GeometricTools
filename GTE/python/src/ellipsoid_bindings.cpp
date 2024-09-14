#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Mathematics/Hyperellipsoid.h>

namespace py = pybind11;

void bind_ellipsoid3d(py::module& m) {
    py::class_<gte::Hyperellipsoid<3, double>>(m, "Ellipsoid3d")
        .def(py::init<>())  // Default constructor
        .def(py::init<gte::Vector<3, double> const&, gte::Vector<3,double> const&>())
        .def_readwrite("center", &gte::Hyperellipsoid<3, double>::center)
        .def_readwrite("axis", &gte::Hyperellipsoid<3, double>::axis)
        .def_readwrite("extent", &gte::Hyperellipsoid<3, double>::extent) 
        // Comparison operators
        .def("__eq__", &gte::Hyperellipsoid<3, double>::operator==)
        .def("__ne__", &gte::Hyperellipsoid<3, double>::operator!=)
        .def("__lt__", &gte::Hyperellipsoid<3, double>::operator<)
        .def("__le__", &gte::Hyperellipsoid<3, double>::operator<=)
        .def("__gt__", &gte::Hyperellipsoid<3, double>::operator>)
        .def("__ge__", &gte::Hyperellipsoid<3, double>::operator>=);
}

