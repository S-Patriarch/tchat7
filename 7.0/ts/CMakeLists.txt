cmake_minimum_required(VERSION 3.5)

project(ts)

set(ts_VERSION_MAJOR 7)
set(ts_VERSION_MINOR 0)
set(ts_VERSION "${ts_VERSION_MAJOR}.${ts_VERSION_MINOR}")

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(SOURCE
    main.cpp
    ../src/info.cpp
    ../src/funct.cpp
    ../src/usingdb.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    -lmariadbclient
    -lz
    -lm
    -ldl

    -lssl
    -lcrypto

    -lpthread
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
