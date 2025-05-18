cmake_minimum_required(VERSION 3.10)

set(LIB_NAME quad)

add_library(${LIB_NAME} SHARED
    src/Quad.cpp
    src/Box3D.cpp
)

target_link_libraries(
    ${LIB_NAME}
    PRIVATE
    rayMath
)

target_include_directories(${LIB_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        include
)
