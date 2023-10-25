cmake_minimum_required(VERSION 3.5)

project(ts)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

include_directories(../include)
set(SOURCES main.cpp
            ../src/info.cpp
            ../src/funct.cpp
            ../src/usingdb.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE -lmariadbclient -lpthread -lz -lm -ldl)
