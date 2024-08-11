#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <sstream>
#include "Mathematics/Matrix.h"

namespace py = pybind11;

void bind_matrix3x3d(py::module& m) {
    py::class_<gte::Matrix<3, 3, double>>(m, "Matrix3x3d")
        .def(py::init<>())  // Default constructor
        .def(py::init<std::array<double, 9> const&>())  // Constructor with array
        .def(py::init<std::initializer_list<double>>())  // Constructor with initializer list
        .def(py::init<int32_t, int32_t>())  // Constructor with row and column for unit matrix
        .def("MakeZero", &gte::Matrix<3, 3, double>::MakeZero)  // MakeZero method
        .def("MakeUnit", &gte::Matrix<3, 3, double>::MakeUnit)  // MakeUnit method
        .def("MakeIdentity", &gte::Matrix<3, 3, double>::MakeIdentity)  // MakeIdentity method
        .def_static("Zero", &gte::Matrix<3, 3, double>::Zero)  // Static Zero method
        .def_static("Unit", &gte::Matrix<3, 3, double>::Unit)  // Static Unit method
        .def_static("Identity", &gte::Matrix<3, 3, double>::Identity)  // Static Identity method
        .def("__getitem__", [](const gte::Matrix<3, 3, double>& mat, std::pair<int, int> idx) {
            return mat(idx.first, idx.second);
        })  // Get item method
        .def("__setitem__", [](gte::Matrix<3, 3, double>& mat, std::pair<int, int> idx, double value) {
            mat(idx.first, idx.second) = value;
        })  // Set item method
        .def("SetRow", &gte::Matrix<3, 3, double>::SetRow)  // SetRow method
        .def("SetCol", &gte::Matrix<3, 3, double>::SetCol)  // SetCol method
        .def("GetRow", &gte::Matrix<3, 3, double>::GetRow)  // GetRow method
        .def("GetCol", &gte::Matrix<3, 3, double>::GetCol)  // GetCol method
        .def("__eq__", &gte::Matrix<3, 3, double>::operator==)  // Equality operator
        .def("__ne__", &gte::Matrix<3, 3, double>::operator!=)  // Inequality operator
        .def("__lt__", &gte::Matrix<3, 3, double>::operator<)  // Less than operator
        .def("__le__", &gte::Matrix<3, 3, double>::operator<=)  // Less than or equal operator
        .def("__gt__", &gte::Matrix<3, 3, double>::operator>)  // Greater than operator
        .def("__ge__", &gte::Matrix<3, 3, double>::operator>=)  // Greater than or equal operator
        .def("__add__", [](const gte::Matrix<3, 3, double>& a, const gte::Matrix<3, 3, double>& b) { return a + b; })  // Addition operator
        .def("__sub__", [](const gte::Matrix<3, 3, double>& a, const gte::Matrix<3, 3, double>& b) { return a - b; })  // Subtraction operator
        .def("__neg__", [](const gte::Matrix<3, 3, double>& a) { return -a; })  // Unary negation operator
        .def("__mul__", [](const gte::Matrix<3, 3, double>& a, double scalar) { return a * scalar; })  // Scalar multiplication operator
        .def("__mul__", [](const gte::Matrix<3, 3, double>& a, const gte::Matrix<3, 3, double>& b) { return a * b; })  // Matrix multiplication operator
        .def("__rmul__", [](double scalar, const gte::Matrix<3, 3, double>& a) { return scalar * a; })  // Right scalar multiplication operator
        .def("__truediv__", [](const gte::Matrix<3, 3, double>& a, double scalar) { return a / scalar; })  // Scalar division operator
        .def("__repr__", [](const gte::Matrix<3, 3, double>& mat) {
            std::ostringstream oss;
            oss << "Matrix3x3d([";
            for (int i = 0; i < 3; ++i) {
                oss << "[";
                for (int j = 0; j < 3; ++j) {
                    oss << mat(i, j);
                    if (j < 2) oss << ", ";
                }
                oss << "]";
                if (i < 2) oss << ", ";
            }
            oss << "])";
            return oss.str();
        });  // Representation method
}

