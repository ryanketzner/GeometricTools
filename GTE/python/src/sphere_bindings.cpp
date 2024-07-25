#include <pybind11/pybind11.h>
#include <Mathematics/Hypersphere.h>

namespace py = pybind11;

void bind_sphere3d(py::module& m) {
    py::class_<gte::Hypersphere<3, double>>(m, "Sphere3d")
        .def(py::init<>())  // Default constructor
        .def(py::init<gte::Vector<3, double> const&, double>())
        .def("Volume", &gte::Hypersphere<3, double>::Volume)
        .def_readwrite("center", &gte::Hypersphere<3, double>::center)
        .def_readwrite("radius", &gte::Hypersphere<3, double>::radius) 
        // Comparison operators
        .def("__eq__", &gte::Hypersphere<3, double>::operator==)
        .def("__ne__", &gte::Hypersphere<3, double>::operator!=)
        .def("__lt__", &gte::Hypersphere<3, double>::operator<)
        .def("__le__", &gte::Hypersphere<3, double>::operator<=)
        .def("__gt__", &gte::Hypersphere<3, double>::operator>)
        .def("__ge__", &gte::Hypersphere<3, double>::operator>=);
}

