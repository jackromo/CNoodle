cmake_minimum_required(VERSION 3.5)
project(cnoodle)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

# Find GLUT

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR "GLUT not found!")
endif(NOT GLUT_FOUND)

# Find OpenGL

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR "OPENGL not found!")
endif(NOT OPENGL_FOUND)

set(SOURCE_FILES main.c cnoodle.h cnd_gamedata.h dispatchers.c gamedata.c)
add_executable(cnoodle ${SOURCE_FILES})

# Link with OpenGL

target_link_libraries(cnoodle ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )