cmake_minimum_required (VERSION 2.6)
project (SimpleRaytracer)

set(ROOT .)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories(${ROOT}/includes)

add_executable(Example main.cpp)

target_link_libraries(Example freeimage)