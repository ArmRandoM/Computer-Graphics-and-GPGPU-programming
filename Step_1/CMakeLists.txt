project(representation)
cmake_minimum_required(VERSION 2.8)

set(PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")
set(PROJECT_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/")
set(GLAD_SOURCES "${CMAKE_SOURCE_DIR}/../glad/src/glad.c")
set(GLAD_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/../glad/include")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
set (OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL  REQUIRED)

include_directories(
  ${GLFW_INCLUDE_DIRS}
  ${GLAD_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  )
  
file(GLOB SOURCES ${PROJECT_SOURCES} ${GLAD_SOURCES})

if(NOT MSVC)
  find_library(M_LIB m)
else() # not needed on Windows
  set(M_LIB "")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(
  ${PROJECT_NAME}
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${M_LIB}
  ${CMAKE_DL_LIBS}
  )
