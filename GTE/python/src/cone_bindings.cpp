#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Mathematics/Ray.h"
#include "Mathematics/Vector3.h"
#include "Mathematics/Cone.h"  // Assuming your Cone class is in this header file

namespace py = pybind11;

void bind_cone3d(py::module& m)
{
    py::class_<gte::Cone<3, double>>(m, "Cone3d")
        .def(py::init<>())  // Default constructor
        .def(py::init<gte::Ray<3, double> const&, double>())  // Constructor with ray and angle
        .def(py::init<gte::Ray<3, double> const&, double, double>())  // Constructor with ray, angle, and min height
        .def(py::init<gte::Ray<3, double> const&, double, double, double>())  // Constructor with ray, angle, min height, and max height
        .def("SetAngle", &gte::Cone<3, double>::SetAngle) 
        .def("MakeInfiniteCone", &gte::Cone<3, double>::MakeInfiniteCone) 
        .def("MakeInfiniteTruncatedCone", &gte::Cone<3, double>::MakeInfiniteTruncatedCone)  
        .def("MakeFiniteCone", &gte::Cone<3, double>::MakeFiniteCone)  
        .def("MakeConeFrustum", &gte::Cone<3, double>::MakeConeFrustum) 
        .def("GetMinHeight", &gte::Cone<3, double>::GetMinHeight)  
        .def("GetMaxHeight", &gte::Cone<3, double>::GetMaxHeight) 
        .def("HeightInRange", &gte::Cone<3, double>::HeightInRange)
        .def("HeightLessThanMin", &gte::Cone<3, double>::HeightLessThanMin)
        .def("HeightGreaterThanMax", &gte::Cone<3, double>::HeightGreaterThanMax)
        .def("IsFinite", &gte::Cone<3, double>::IsFinite)
        .def("IsInfinite", &gte::Cone<3, double>::IsInfinite)
        .def_readwrite("ray", &gte::Cone<3, double>::ray)
        .def_readwrite("angle", &gte::Cone<3, double>::angle)
        .def_readwrite("cosAngle", &gte::Cone<3, double>::cosAngle)
        .def_readwrite("sinAngle", &gte::Cone<3, double>::sinAngle)
        .def_readwrite("tanAngle", &gte::Cone<3, double>::tanAngle)
        .def_readwrite("cosAngleSqr", &gte::Cone<3, double>::cosAngleSqr)
        .def_readwrite("sinAngleSqr", &gte::Cone<3, double>::sinAngleSqr)
        .def_readwrite("invSinAngle", &gte::Cone<3, double>::invSinAngle)
        // Comparison operators
        .def("__eq__", &gte::Cone<3, double>::operator==)
        .def("__ne__", &gte::Cone<3, double>::operator!=)
        .def("__lt__", &gte::Cone<3, double>::operator<)
        .def("__le__", &gte::Cone<3, double>::operator<=)
        .def("__gt__", &gte::Cone<3, double>::operator>)
        .def("__ge__", &gte::Cone<3, double>::operator>=);
}
