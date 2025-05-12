cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fPIC -O3")


add_subdirectory(Sphere)
add_subdirectory(CustomShapes)
add_subdirectory(Triangle)
add_subdirectory(Cylinder)
add_subdirectory(Lambertarian)
add_subdirectory(Plane)
