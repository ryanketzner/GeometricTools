#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_vector3d(py::module& m);
void bind_halfspace3d(py::module& m);
void bind_sphere3d(py::module& m);
void bind_rectview3d(py::module& m);
void bind_cone3d(py::module& m);
void bind_matrix3x3d(py::module& m);
void bind_euleranglesd(py::module& m);
void bind_rotationd(py::module& m);

PYBIND11_MODULE(GeometricTools, m)
{
    bind_vector3d(m);
    bind_halfspace3d(m);
    bind_sphere3d(m);
    bind_rectview3d(m);
    bind_cone3d(m);
    bind_matrix3x3d(m);
    bind_euleranglesd(m);
    bind_rotationd(m);
}

