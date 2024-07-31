#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Mathematics/Vector3.h"
#include "Mathematics/Halfspace.h"
#include "Mathematics/Ray.h"
#include "Mathematics/RectView3.h"

namespace py = pybind11;

void bind_rectview3d(py::module& m)
{
    py::class_<gte::RectView3<double>>(m, "RectView3d")
        .def(py::init<>())  // Default constructor
        .def(py::init<gte::Vector3<double> const&, gte::Vector3<double> const&, gte::Vector3<double> const&, double, double>()) 
        .def("SetAngleHeight", &gte::RectView3<double>::SetAngleHeight)
        .def("SetAngleWidth", &gte::RectView3<double>::SetAngleWidth)
        .def("Update", &gte::RectView3<double>::Update)
        .def("GetAngleHeight", &gte::RectView3<double>::GetAngleHeight)
        .def("GetAngleWidth", &gte::RectView3<double>::GetAngleWidth)
        .def_readwrite("uVector", &gte::RectView3<double>::uVector)  
        .def_readwrite("rVector", &gte::RectView3<double>::rVector)
        .def_readwrite("vertex", &gte::RectView3<double>::vertex)
        .def_readwrite("halfspaces", &gte::RectView3<double>::halfspaces); 
}
