project(pghp-td6)

cmake_minimum_required(VERSION 2.6)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(Qt4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Eigen3 "3.0.3" REQUIRED)

set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories( ${QT_INCLUDE_DIR} )
include_directories( ${EIGEN3_INCLUDE_DIR} )
include_directories( ${PROJECT_SOURCE_DIR}/include )
include_directories( ${PROJECT_SOURCE_DIR}/src )

add_definitions(-DPGHP_DIR="${PROJECT_SOURCE_DIR}")
add_definitions(-DEIGEN_DONT_ALIGN_STATICALLY)

if(APPLE)
  set(APPLE_WORKAROUND_SRCS src/core_profile_attributes.mm)
endif()

#if(NOT CUDA_INSTALL_PREFIX)
#  set(CUDA_INSTALL_PREFIX /usr/local/cuda)
#endif()
#find_package(Cuda REQUIRED)

#include_directories(${CUDA_INSTALL_PREFIX}/include)
#cuda_include_directories(
#    ${CMAKE_BINARY_DIR}
#    ${QT_INCLUDES}
#    ${CUDA_INSTALL_PREFIX}/include
#    ${PROJECT_SOURCE_DIR}/include
#)

#if(CUDA_FOUND)
#  message(STATUS "Found Cuda")
#else()
#  message(STATUS "Cuda not found")
#endif()

# the list of source files
set(pghp_SRCS
  ObjFormat/ObjFormat.cpp
  ObjFormat/ObjUtil.cpp
  src/RenderingWidget.cpp
  src/Mesh.cpp
  src/Shader.cpp
  src/Camera.cpp
  src/Object.cpp
  src/main.cpp
  src/graph.cpp
  ${APPLE_WORKAROUND_SRCS})

qt4_automoc(${pghp_SRCS})

add_executable(springEmbedders ${pghp_SRCS})

target_link_libraries(springEmbedders
  ${QT_QTCORE_LIBRARY}    ${QT_QTGUI_LIBRARY}
  ${QT_QTOPENGL_LIBRARY}  ${OPENGL_LIBRARIES}
)

if(APPLE)
  target_link_libraries(springEmbedders "-framework Foundation" "-framework Cocoa")
endif()
