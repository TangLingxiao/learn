cmake_minimum_required(VERSION 2.6)

project(snet C CXX)

set(CXX_FLAGS
 -g
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 -pthread
 -lsnet
 )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../net/poller/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} SOURCE_FILES)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test SOURCE_FILES)


# ADD_EXECUTABLE(test_eventloop.out ${CMAKE_CURRENT_SOURCE_DIR}/test_eventloop.cpp)
# target_link_libraries(test_eventloop.out snet)

# ADD_EXECUTABLE(test_poller.out ${CMAKE_CURRENT_SOURCE_DIR}/test_poller.cpp)
# target_link_libraries(test_poller.out snet)

# ADD_EXECUTABLE(test_timerqueue.out ${CMAKE_CURRENT_SOURCE_DIR}/test_timerqueue.cpp)
# target_link_libraries(test_timerqueue.out snet)

# ADD_EXECUTABLE(test_eventloopthread.out ${CMAKE_CURRENT_SOURCE_DIR}/test_eventloopthread.cpp)
# target_link_libraries(test_eventloopthread.out snet)

# ADD_EXECUTABLE(test_eventloopthreadpool.out ${CMAKE_CURRENT_SOURCE_DIR}/test_eventloopthreadpool.cpp)
# target_link_libraries(test_eventloopthreadpool.out snet)

# ADD_EXECUTABLE(test_acceptor.out ${CMAKE_CURRENT_SOURCE_DIR}/test_acceptor.cpp)
# target_link_libraries(test_acceptor.out snet)

# ADD_EXECUTABLE(test_connector.out ${CMAKE_CURRENT_SOURCE_DIR}/test_connector.cpp)
# target_link_libraries(test_connector.out snet)

ADD_EXECUTABLE(test_tcpserver.out ${CMAKE_CURRENT_SOURCE_DIR}/test_tcpserver.cpp)
target_link_libraries(test_tcpserver.out snet)

ADD_EXECUTABLE(test_tcpclient.out ${CMAKE_CURRENT_SOURCE_DIR}/test_tcpclient.cpp)
target_link_libraries(test_tcpclient.out snet)