cmake_minimum_required(VERSION 3.10)
project(QmkFormatConverter C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -pedantic -std=c17 -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/logger)

add_executable(QmkFormatConverter src/main.c)

target_link_libraries(QmkFormatConverter PUBLIC QfcLogger)
target_include_directories(QfcLogger PUBLIC
                          "${PROJECT_SOURCE_DIR}/src/logger"
                          )



