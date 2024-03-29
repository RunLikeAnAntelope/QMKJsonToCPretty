cmake_minimum_required(VERSION 3.10)
project(QmkFormatConverter C)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/logger)

target_include_directories(QfcLogger PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src/logger"
                          )

add_executable(QmkFormatConverter src/main.c)
target_link_libraries(QmkFormatConverter PUBLIC QfcLogger)


