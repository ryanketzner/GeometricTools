#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Mathematics/Rotation.h"

namespace py = pybind11;

// Add better conversion functions later.
void bind_rotationd(py::module& m)
{
    py::class_<gte::Rotation<3, double>>(m, "Rotationd")
        .def(py::init<gte::Matrix<3, 3, double> const&>())
        .def(py::init<gte::Quaternion<double> const&>())
        .def(py::init<gte::AxisAngle<3, double> const&>())
        .def(py::init<gte::EulerAngles<double> const&>())
        .def("to_matrix", [](const gte::Rotation<3, double>& r) {
            return static_cast<gte::Matrix<3, 3, double>>(r);
        })
        .def("to_quaternion", [](const gte::Rotation<3, double>& r) {
            return static_cast<gte::Quaternion<double>>(r);
        })
        .def("to_axis_angle", [](const gte::Rotation<3, double>& r) {
            return static_cast<gte::AxisAngle<3, double>>(r);
        })
        .def("to_euler_angles", [](const gte::Rotation<3, double>& r, int32_t i0, int32_t i1, int32_t i2) {
            return r(i0, i1, i2);
        });
}
