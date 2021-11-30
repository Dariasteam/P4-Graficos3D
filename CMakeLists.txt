cmake_minimum_required(VERSION 3.21)
project(p3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/auxiliar.cpp
    src/Light.cpp
    src/Camera.cpp
    src/Scene.cpp
    src/ShaderManager.cpp
    src/OpenGLManager.cpp
    src/SceneManager.cpp
    src/TextureManager.cpp
    src/main.cpp
)

#Copy aux files to build directory
foreach(file meshes img shaders_P3)
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/${file} )
    message("copy " ${CMAKE_SOURCE_DIR}/${file}  " to " ${CMAKE_BINARY_DIR} )
    file(COPY ${CMAKE_SOURCE_DIR}/${file} DESTINATION ${CMAKE_BINARY_DIR} )
endif()
endforeach()


add_definitions ("-Wall -L../glm/glm/ -lGLEW -lGL -lglut -lGLU -lfreeimage")
add_definitions ("-std=c++1z")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}
                     ${GLUT_INCLUDE_DIRS}
                     ${GLEW_LIBRARIES}
                     ${FREEIMAGE_INCLUDE_DIRS}
                     ${ASSIMP_INCLUDE_DIRS})

add_executable(p3 ${SOURCES})


target_link_libraries(p3 ${OPENGL_LIBRARIES}
                         ${GLUT_LIBRARY}
                         ${GLEW_LIBRARIES}
                         ${FREEIMAGE_INCLUDE_DIRS}
                         ${ASSIMP_LIBRARIES}
                         "-lfreeimage"
                         "-lglut") #Un poco chapuza pero no me quería complicar

