#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Mathematics/Vector.h"

namespace py = pybind11;

void bind_vector3d(py::module& m) {
    py::class_<gte::Vector<3, double>>(m, "Vector3d")
        .def(py::init<>())  // Default constructor
        .def(py::init<std::array<double, 3> const&>())
        .def(py::init<std::initializer_list<double>>())
        .def(py::init<int32_t>())
        .def("GetSize", &gte::Vector<3, double>::GetSize)
        .def("__getitem__", [](gte::Vector<3, double> &v, int i) -> double& {
            if (i < 0 || i >= v.GetSize()) throw py::index_error();
            return v[i];
        }, py::return_value_policy::reference_internal)
        .def("__setitem__", [](gte::Vector<3, double> &v, int i, double val) {
            if (i < 0 || i >= v.GetSize()) throw py::index_error();
            v[i] = val;
        })
        // Comparison operators
        .def("__eq__", &gte::Vector<3, double>::operator==)
        .def("__ne__", &gte::Vector<3, double>::operator!=)
        .def("__lt__", &gte::Vector<3, double>::operator<)
        .def("__le__", &gte::Vector<3, double>::operator<=)
        .def("__gt__", &gte::Vector<3, double>::operator>)
        .def("__ge__", &gte::Vector<3, double>::operator>=)
        // Methods to set special vector states
        .def("MakeZero", &gte::Vector<3, double>::MakeZero)
        .def("MakeOnes", &gte::Vector<3, double>::MakeOnes)
        .def("MakeUnit", &gte::Vector<3, double>::MakeUnit)
        // Static methods to create special vectors
        .def_static("Zero", &gte::Vector<3, double>::Zero)
        .def_static("Ones", &gte::Vector<3, double>::Ones)
        .def_static("Unit", &gte::Vector<3, double>::Unit);
}

