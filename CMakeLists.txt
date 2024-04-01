cmake_minimum_required(VERSION 3.10)
project(QmkFormatConverter C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -pedantic -std=gnu17 -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/logger)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/QJTCP)

add_executable(QmkFormatConverter src/main.c)

target_link_libraries(QmkFormatConverter PUBLIC QfcLogger QJTCP )
target_include_directories(QJTCP PUBLIC
                          "${PROJECT_SOURCE_DIR}/src/logger"
                          "${PROJECT_SOURCE_DIR}/src/QJTCP"
                          )



