#include <pybind11/pybind11.h>
#include "Mathematics/Halfspace.h"

namespace py = pybind11;

void bind_halfspace3d(py::module& m) {
    py::class_<gte::Halfspace<3, double>>(m, "Halfspace3d")
        .def(py::init<>())  // Default constructor
        .def(py::init<gte::Vector<3, double> const&, double>()) 
        .def_readwrite("normal", &gte::Halfspace<3, double>::normal) 
        .def_readwrite("constant", &gte::Halfspace<3, double>::constant) 
        // Comparison operators
        .def("__eq__", &gte::Halfspace<3, double>::operator==)
        .def("__ne__", &gte::Halfspace<3, double>::operator!=)
        .def("__lt__", &gte::Halfspace<3, double>::operator<)
        .def("__le__", &gte::Halfspace<3, double>::operator<=)
        .def("__gt__", &gte::Halfspace<3, double>::operator>)
        .def("__ge__", &gte::Halfspace<3, double>::operator>=);
}

