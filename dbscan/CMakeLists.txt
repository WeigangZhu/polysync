CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#project name
PROJECT(DBSCAN)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 OLD)
	cmake_policy(SET CMP0015 OLD)
endif(COMMAND cmake_policy)

add_definitions(-Wall -lpthread -O3)

SET(execName dbscan)



SET(EXECUTABLE_OUTPUT_PATH ".")

#head file path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(SRC_FOLDER src)
FILE(GLOB_RECURSE SRC_FILES  "${SRC_FOLDER}/*.c")

#set extern libraries
SET(LIBRARIES libm.so)

ADD_EXECUTABLE(${execName} ${SRC_FILES})

TARGET_LINK_LIBRARIES(${execName} ${LIBRARIES})

