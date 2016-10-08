project(d2d-conv-console-svc CXX)


# Find required modules
# PKG_CHECK_MODULES: used to check for given modules
# PKG_SEARCH_MODULE: used to check for given modules and uses the first working one
INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(rpkgs-dlog REQUIRED dlog)
PKG_CHECK_MODULES(rpkgs-glib REQUIRED glib-2.0)
PKG_CHECK_MODULES(rpkgs-d2d REQUIRED d2d-conv-manager)


# Add CFLAGS used for required modules
ADD_DEFINITIONS(${rpkgs-dlog_CFLAGS})
ADD_DEFINITIONS(${rpkgs-glib_CFLAGS})
ADD_DEFINITIONS(${rpkgs-d2d_CFLAGS})

# Set include directories
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${rpkgs-dlog_INCLUDE_DIRS}
	${rpkgs-d2d_INCLUDE_DIRS}
	)

# Set sources
ADD_EXECUTABLE(${PROJECT_NAME}
	src/common.cpp
	src/main.cpp
	src/d2d_conv_console.cpp
)

# Set LDFLAGS
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${rpkgs-dlog_LDFLAGS} ${rpkgs-glib_LDFLAGS} ${rpkgs-d2d_LDFLAGS} "-ldl -lrt")

# Set CFLAGS
ADD_DEFINITIONS(-Wall -O3)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=C++0x")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -omit-frame-pointer -std=gnu++0x")

# Move result binary to the target position(/usr/bin)
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION bin)
