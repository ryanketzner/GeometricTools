#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_vector3d(py::module& m);
void bind_halfspace3d(py::module& m);
void bind_sphere3d(py::module& m);

PYBIND11_MODULE(GeometricTools, m)
{
    bind_vector3d(m);
    bind_halfspace3d(m);
    bind_sphere3d(m);
}

