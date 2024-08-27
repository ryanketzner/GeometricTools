#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Mathematics/Vector3.h"
#include "Mathematics/PointS2.h"

namespace py = pybind11;

void bind_points2(py::module& m)
{
    py::class_<gte::PointS2<double>>(m, "PointS2d")
        .def(py::init<>())  // Default constructor
        .def(py::init<double, double, bool, bool>(),  // Constructor with default arguments
             py::arg("lat"),                    // Default value for lat
             py::arg("lon"),                    // Default value for lon
             py::arg("enforce_bounds") = true,        // Default value for enforce_bounds
             py::arg("degrees") = false)              // Default value for degrees
        .def("Lat", &gte::PointS2<double>::Lat)
        .def("Lon", &gte::PointS2<double>::Lon);
    
    m.def("GeographicToCart", &gte::GeographicToCart<double>, 
          py::arg("point"), py::arg("radius") = 1.0, "Return point in Cartesian coordinates.");
    
    m.def("CartToGeographic", &gte::CartToGeographic<double>, 
          py::arg("vector"), "Return point in Geographic coordinates.");
}

