# Reqired CMake version and project name
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(d2d-conv-console)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")
SET(VERSION 1.0)

# Add projects in subdirectory
ADD_SUBDIRECTORY(console)
