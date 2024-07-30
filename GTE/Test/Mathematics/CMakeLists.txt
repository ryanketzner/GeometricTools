# Get all the .cpp files in the current directory
file(GLOB CPP_FILES *.cpp)

set(GTE_ROOT ${PROJECT_SOURCE_DIR}/..)
set(GTE_INCLUDE_DIR ${GTE_ROOT})

include_directories(${GTE_INCLUDE_DIR})

find_package(GTest REQUIRED)
set(LIBS GTest::GTest GTest::Main)

enable_testing()

# Iterate through the .cpp files, creating executables and linking libraries
foreach(file ${CPP_FILES})
    get_filename_component(target_name ${file} NAME_WE)
    add_executable(${target_name} ${file})
    target_link_libraries(${target_name} ${LIBS})
    add_test(${target_name} ${target_name})
endforeach()
