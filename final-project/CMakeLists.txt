cmake_minimum_required (VERSION 2.8)
project (FinalProject)

# Make a define, only used with Linux.  Also, make sure we use c++11
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__LINUX__ -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LINUX__ -std=c++14")

# Linux is very good with package management and users should understand about the
# highly organized system they run on.  Either way, depend on the libraries
# being installed on the system.  Arch Linux has everything in the repos or AUR

# Required Libraries
#
# OpenGL Runtime             (Mesa-libGL1) - http://www.mesa3d.org/
# OpenGL Extension Wrangler  (GLEW)        - http://glew.sourceforge.net/
# OpenGL Mathematics Library (GLM)         - http://glm.g-truc.net/
# OpenGL Utilities Library   (GLU)         - 
# JsonCPP library            (jsoncpp)     - https://github.com/open-source-parsers/jsoncpp
#
# Simple Direct Media Layer (requires 2.0.0 or greater) - http://libsdl.org/download-2.0.php
#
# A graphics card and driver supporting at least OpenGL 4.4
#
# Graphics cards known to work
#     Nvidia GeForce GTX 950 (4.5.0 NVIDIA 364.19)
#     Nvidia GeForce GTX 980 (4.5.0 NVIDIA 352.63)
#

# Discover include path and library for the SDL2 library
find_path(SDL_INCLUDE_DIR NAMES SDL2/SDL.h)
include_directories(${SDL_INCLUDE_DIR})
find_library(SDL_LIBRARY NAMES SDL2)

# Discover the include path and library for the system OpenGL Library
find_path(OPENGL_INCLUDE_DIR NAMES GL/gl.h)
include_directories(${OPENGL_INCLUDE_DIR})
find_library(OPENGL_LIBRARY NAMES GL)

# Discover the include path and library for the GLU Library
find_path(GLU_INCLUDE_DIR NAMES GL/glu.h)
include_directories(${GLU_INCLUDE_DIR})
find_library(GLU_LIBRARY NAMES GLU)

# Discover the include path and library for the OpenGL Extension Wrangler
find_path(GLEW_INCLUDE_DIR NAMES GL/glew.h)
include_directories(${GLEW_INCLUDE_DIR})
find_library(GLEW_LIBRARY NAMES GLEW)

# Discover the include path for the OpenGL Mathematics Library
find_path(GLM_INCLUDE_DIR NAMES glm/glm.hpp)
include_directories(${GLM_INCLUDE_DIR})

# Add universal includes
include_directories("include")

# Project Files
file(GLOB Sources
    "src/*.cpp"
    "src/objects/*.cpp"

    "destructo-base/*.cpp"
    "destructo-base/*.hpp"
    "destructo-base/*.h"
    
    "include/project/*.hpp"
    "include/project/objects/*.hpp"
)

# FinalProject Executable
add_executable(FinalProject ${Sources})
target_link_libraries(FinalProject ${SDL_LIBRARY})
target_link_libraries(FinalProject ${OPENGL_LIBRARY})
target_link_libraries(FinalProject ${GLU_LIBRARY})
target_link_libraries(FinalProject ${GLEW_LIBRARY})
target_link_libraries(FinalProject pthread)
