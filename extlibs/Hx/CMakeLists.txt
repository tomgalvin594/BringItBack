cmake_minimum_required(VERSION 2.8)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()


# -----------------------------------------
#  Setup the main executable
# -----------------------------------------

project(Hx)

add_subdirectory(include)
add_subdirectory(src)

# Global preprocessor and include options
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/Hx/Depot)
include_directories(${PROJECT_SOURCE_DIR}/include/Hx/Depot/SFML)
include_directories(${PROJECT_SOURCE_DIR}/include/Hx/Kairos)
include_directories(${PROJECT_SOURCE_DIR}/include/Hx/Plinth)
include_directories(${PROJECT_SOURCE_DIR}/include/Hx/Plinth/Bezier)
include_directories(${PROJECT_SOURCE_DIR}/include/Hx/Plinth/SFML)

# Setup the executable
set(LIBRARY_NAME "Hx")
add_library(${LIBRARY_NAME} STATIC ${PROJECT_SOURCES} ${PROJECT_INCLUDES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Pass on the install path
set(HX_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/lib${LIBRARY_NAME}.a PARENT_SCOPE)

# Add directory containing FindSFML.cmake to module path
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}")

# Find SFML
find_package(SFML 2 COMPONENTS audio graphics window system)

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
    #target_link_libraries(${LIBRARY_NAME} ${SFML_LIBRARIES})
endif()

