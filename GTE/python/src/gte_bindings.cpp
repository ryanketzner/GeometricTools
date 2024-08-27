#include <pybind11/pybind11.h>
#include "vector_bindings.hpp"
#include "randomsurfacegrid_bindings.hpp"

namespace py = pybind11;

//void bind_vector3d(py::module& m);
void bind_halfspace3d(py::module& m);
void bind_sphere3d(py::module& m);
void bind_rectview3d(py::module& m);
void bind_cone3d(py::module& m);
void bind_matrix3x3d(py::module& m);
void bind_euleranglesd(py::module& m);
void bind_rotationd(py::module& m);
void bind_points2(py::module& m);

PYBIND11_MODULE(GeometricTools, m)
{
    //bind_vectorNd<1>(m,"Vector1d");
    bind_vectorNd<2>(m,"Vector2d");
    bind_vectorNd<3>(m,"Vector3d");
    bind_vectorNd<4>(m,"Vector4d");
    bind_vectorNd<5>(m,"Vector5d");
    bind_vectorNd<6>(m,"Vector6d");
    bind_vectorNd<7>(m,"Vector7d");
    bind_vectorNd<8>(m,"Vector8d");
    bind_vectorNd<9>(m,"Vector9d");

    bind_halfspace3d(m);
    bind_sphere3d(m);
    bind_rectview3d(m);
    bind_cone3d(m);
    bind_matrix3x3d(m);
    bind_euleranglesd(m);
    bind_rotationd(m);
    bind_points2(m);

    bind_random_surface_grid<3,double>(m,"RandomSurfaceGrid3d");
    
}

