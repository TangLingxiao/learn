cmake_minimum_required(VERSION 2.6)

project(test C CXX)
enable_language(C ASM)
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
 -Wno-unused-function
 #-Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 #-Wshadow
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

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Interface)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Log/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Pattern/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Util/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Async/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Structure/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Timer/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Pool/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Net/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../Net/Poller)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/include/hiredis)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../DB/Redis/Test)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/include/libco)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/lib)

#ADD_EXECUTABLE(test_log.out ${CMAKE_CURRENT_SOURCE_DIR}/test_log.cpp)
#ADD_EXECUTABLE(test_async.out ${CMAKE_CURRENT_SOURCE_DIR}/test_async.cpp)
#ADD_EXECUTABLE(test_timer.out ${CMAKE_CURRENT_SOURCE_DIR}/test_timer.cpp)
#target_link_libraries(test_log.out mess)
#target_link_libraries(test_async.out mess)
#target_link_libraries(test_timer.out mess)
#
ADD_EXECUTABLE(test_server.out ${CMAKE_CURRENT_SOURCE_DIR}/test_server.cpp)
target_link_libraries(test_server.out mess)
#
#ADD_EXECUTABLE(test_client.out ${CMAKE_CURRENT_SOURCE_DIR}/test_client.cpp)
#target_link_libraries(test_client.out mess)

ADD_EXECUTABLE(test_hook.out ${CMAKE_CURRENT_SOURCE_DIR}/test_hook.cpp)
target_link_libraries(test_hook.out mess)

#ADD_EXECUTABLE(test_redis.out ${CMAKE_CURRENT_SOURCE_DIR}/test_redis.cpp)
#target_link_libraries(test_redis.out mess hiredis dl colib)
#
#ADD_EXECUTABLE(test_co.out ${CMAKE_CURRENT_SOURCE_DIR}/test_co.cpp)
#target_link_libraries(test_co.out mess colib pthread dl)