#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Mathematics/EulerAngles.h"

namespace py = pybind11;

void bind_euleranglesd(py::module& m) {
    py::enum_<gte::EulerResult>(m, "EulerResult")
        .value("INVALID", gte::EulerResult::INVALID)
        .value("UNIQUE", gte::EulerResult::UNIQUE)
        .value("NOT_UNIQUE_SUM", gte::EulerResult::NOT_UNIQUE_SUM)
        .value("NOT_UNIQUE_DIF", gte::EulerResult::NOT_UNIQUE_DIF)
        .export_values();

    py::class_<gte::EulerAngles<double>>(m, "EulerAnglesd")
        .def(py::init<>())  // Default constructor
        .def(py::init<int32_t, int32_t, int32_t, double, double, double>())  // Constructor with parameters
        .def_readwrite("axis", &gte::EulerAngles<double>::axis)  // axis member
        .def_readwrite("angle", &gte::EulerAngles<double>::angle)  // angle member
        .def_readwrite("result", &gte::EulerAngles<double>::result)  // result member
        .def("__repr__", [](const gte::EulerAngles<double>& ea) {
            return "EulerAnglesd(axis=[" + std::to_string(ea.axis[0]) + ", " + std::to_string(ea.axis[1]) + ", " + std::to_string(ea.axis[2]) +
                   "], angle=[" + std::to_string(ea.angle[0]) + ", " + std::to_string(ea.angle[1]) + ", " + std::to_string(ea.angle[2]) +
                   "], result=" + std::to_string(static_cast<int>(ea.result)) + ")";
        });
}
