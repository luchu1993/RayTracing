cmake_minimum_required(VERSION 2.10)

project(RayTracing)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

include_directories(include)

add_executable(output_an_image src/ch01_output_an_image.cpp src/rt_math.cpp)
add_executable(ray src/ch03_ray.cpp src/rt_math.cpp)
add_executable(sphere src/ch04_sphere.cpp src/rt_math.cpp)
add_executable(normal src/ch05_normal.cpp src/rt_math.cpp)
add_executable(multiple_objects src/ch05_multiple_objects.cpp src/rt_math.cpp)
add_executable(antialiasing src/ch06_antialiasing.cpp src/rt_math.cpp)
add_executable(diffuse src/ch07_diffuse.cpp src/rt_math.cpp)
add_executable(material src/ch08_material.cpp src/rt_math.cpp)
add_executable(dielectrics src/ch09_dielectrics.cpp src/rt_math.cpp)
add_executable(camera src/ch10_camera.cpp src/rt_math.cpp)
