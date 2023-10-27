cmake_minimum_required(VERSION 3.5)

project(ts)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ts_VERSION_MAJOR 7)
set(ts_VERSION_MINOR 0)
set(ts_VERSION "${ts_VERSION_MAJOR}.${ts_VERSION_MINOR}")

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_SOURCE_DIR}/../include)
set(SOURCE_CPP main.cpp
               ../src/info.cpp
               ../src/funct.cpp
               ../src/usingdb.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_CPP})

target_link_libraries(${PROJECT_NAME} PRIVATE -lmariadbclient
                                              -lpthread
                                              -lz
                                              -lm
                                              -ldl
                                              -lpthread)
