cmake_minimum_required(VERSION 3.8)
project(rt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

add_subdirectory(external/glfw-3.3.2)
include_directories(include)

set(LIBS glfw X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread)

add_library(GLAD src/glad.c)
set(LIBS ${LIBS} GLAD)

add_executable(rt src/picture.cpp src/Bitmap.cpp)

target_link_libraries(rt ${LIBS})

file(GLOB SHADERS "src/shaders/*.glsl")
foreach(SHADER ${SHADERS})
    file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)
endforeach(SHADER)