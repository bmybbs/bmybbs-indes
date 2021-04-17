cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(BMYBBS-java)

if(EXISTS ${CMAKE_SOURCE_DIR}/config.cmake)
	include(${CMAKE_SOURCE_DIR}/config.cmake)
	message(STATUS "Found config.cmake")
endif(EXISTS ${CMAKE_SOURCE_DIR}/config.cmake)

set(CMAKE_C_FLAGS "-Wall -g -pipe -Wextra -std=gnu11")

if(NOT DEFINED JAVA_INCLUDE)
	message(FATAL_ERROR "You haven't set the variable JAVA_INCLUDE, please check out config.sample.cmake")
endif(NOT DEFINED JAVA_INCLUDE)

if(NOT DEFINED BMYBBS_INCLUDE)
	message(FATAL_ERROR "You haven't set the variable BMYBBS_INCLUDE, please check out config.sample.cmake")
endif(NOT DEFINED BMYBBS_INCLUDE)

if(NOT DEFINED BMYBBS_LIBRARIES)
	message(FATAL_ERROR "You haven't set the variable BMYBBS_LIBRARIES, please check out config.sample.cmake")
endif(NOT DEFINED BMYBBS_LIBRARIES)

include_directories(include ${JAVA_INCLUDE} ${BMYBBS_INCLUDE})
link_directories(${BMYBBS_LIBRARIES})

add_subdirectory(src)
