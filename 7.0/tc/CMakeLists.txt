cmake_minimum_required(VERSION 3.5)

project(tc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

include_directories(../include)
set(SOURCES main.cpp
            ../src/info.cpp
            ../src/funct.cpp
            ../src/cuser.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
