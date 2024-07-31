#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Mathematics/Vector.h"

namespace py = pybind11;

template <size_t N>
void bind_vectorNd(py::module& m, const std::string& name)
{
    py::class_<gte::Vector<N, double>>(m, name.c_str())
        .def(py::init<>())  // Default constructor
        .def(py::init<std::array<double, N> const&>())
        .def(py::init<std::initializer_list<double>>())
        .def(py::init<int32_t>())
        .def("GetSize", &gte::Vector<N, double>::GetSize)
        .def("__getitem__", [](gte::Vector<N, double> &v, int i) -> double& {
            if (i < 0 || i >= v.GetSize()) throw py::index_error();
            return v[i];
        }, py::return_value_policy::reference_internal)
        .def("__setitem__", [](gte::Vector<N, double> &v, int i, double val) {
            if (i < 0 || i >= v.GetSize()) throw py::index_error();
            v[i] = val;
        })
        // Comparison operators
        .def("__eq__", &gte::Vector<N, double>::operator==)
        .def("__ne__", &gte::Vector<N, double>::operator!=)
        .def("__lt__", &gte::Vector<N, double>::operator<)
        .def("__le__", &gte::Vector<N, double>::operator<=)
        .def("__gt__", &gte::Vector<N, double>::operator>)
        .def("__ge__", &gte::Vector<N, double>::operator>=)
        // Methods to set special vector states
        .def("MakeZero", &gte::Vector<N, double>::MakeZero)
        .def("MakeOnes", &gte::Vector<N, double>::MakeOnes)
        .def("MakeUnit", &gte::Vector<N, double>::MakeUnit)
        // Static methods to create special vectors
        .def_static("Zero", &gte::Vector<N, double>::Zero)
        .def_static("Ones", &gte::Vector<N, double>::Ones)
        .def_static("Unit", &gte::Vector<N, double>::Unit);
}