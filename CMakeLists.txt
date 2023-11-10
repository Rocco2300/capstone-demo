cmake_minimum_required(VERSION 3.23)

project(capstone-demo)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} freeglut opengl32 glew32 glu32)