cmake_minimum_required(VERSION 2.6)

project(mess C CXX)

set(CXX_FLAGS
 -g
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 #-Wconversion
 -Wno-unused-parameter
 #-Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 -pthread
 )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Interface)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Log/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Pattern/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Util/)
#set(SOURCE_FILES
#${CMAKE_CURRENT_SOURCE_DIR}/base/logmgr.cpp 
#${CMAKE_CURRENT_SOURCE_DIR}/net/eventloop.cpp
#${CMAKE_CURRENT_SOURCE_DIR}/net/poller.cpp
#${CMAKE_CURRENT_SOURCE_DIR}/net/channel.cpp
#)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Util SOURCE_FILES)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Log SOURCE_FILES)
ADD_LIBRARY(mess ${SOURCE_FILES})