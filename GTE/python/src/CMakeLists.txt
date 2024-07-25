
find_package(pybind11 REQUIRED)

# Specify the output directory for shared libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/python/lib)

add_library(GeometricTools MODULE gte_bindings.cpp vector_bindings.cpp halfspace_bindings.cpp sphere_bindings.cpp)

# Ensure the shared library has the correct name and extension
set_target_properties(GeometricTools PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)
target_include_directories(GeometricTools PRIVATE ${PYBIND11_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
target_link_libraries(GeometricTools PRIVATE pybind11::module)
