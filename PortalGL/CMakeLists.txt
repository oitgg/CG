cmake_minimum_required(VERSION 2.8)

project(portalgl)

file(GLOB op_src
    "src/*.cpp"
)

add_executable(portalgl ${op_src}) 

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ./include ./CML)

target_link_libraries(portalgl ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} -lfreeimage)

