add_library(src Mesh.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(src freeglut opengl32)