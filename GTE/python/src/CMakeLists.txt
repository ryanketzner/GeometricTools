
find_package(pybind11 REQUIRED)

# Specify the output directory for shared libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/python/lib)

add_library(GeometricTools SHARED gte_bindings.cpp halfspace_bindings.cpp sphere_bindings.cpp rectview_bindings.cpp cone_bindings.cpp matrix_bindings.cpp eulerangles_bindings.cpp rotation_bindings.cpp points2_bindings.cpp ellipsoid_bindings.cpp)

# Ensure the shared library has the correct name and extension
set_target_properties(GeometricTools PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)
target_include_directories(GeometricTools PRIVATE ${PYBIND11_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
target_link_libraries(GeometricTools PRIVATE pybind11::module)
