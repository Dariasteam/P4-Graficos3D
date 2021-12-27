cmake_minimum_required(VERSION 3.21)
project(p4)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/aux/auxiliar.cpp
    src/spatial/light/Light.cpp
    src/spatial/camera/Camera.cpp
    src/scene/Scene.cpp
    src/shader/ShaderManager.cpp
    src/ogl/OpenGLManager.cpp
    src/scene/SceneManager.cpp
    src/texture/TextureManager.cpp
    src/main.cpp
)

#Copy aux files to build directory
foreach(file meshes img shaders_P4)
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/${file} )
    message("copy " ${CMAKE_SOURCE_DIR}/${file}  " to " ${CMAKE_BINARY_DIR} )
    file(COPY ${CMAKE_SOURCE_DIR}/${file} DESTINATION ${CMAKE_BINARY_DIR} )
endif()
endforeach()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}
                     ${GLUT_INCLUDE_DIRS}
                     ${GLEW_LIBRARIES}
                     ${FREEIMAGE_INCLUDE_DIRS}
                     ${ASSIMP_INCLUDE_DIRS})

add_executable(p4 ${SOURCES})


target_link_libraries(p4 ${OPENGL_LIBRARIES}
                         ${GLUT_LIBRARY}
                         ${GLEW_LIBRARIES}
                         ${FREEIMAGE_INCLUDE_DIRS}
                         ${ASSIMP_LIBRARIES}
                         "-lfreeimage"
                         "-pthread"
                         "-lglut") #Un poco chapuza pero no me quería complicar

