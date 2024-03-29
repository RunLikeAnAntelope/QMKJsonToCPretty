cmake_minimum_required(VERSION 3.10)
project(QMKJsonToCPretty C)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES src/main.c src/qflogger.h src/qflogger.c)
add_executable(QMKJsonToCPretty ${SOURCES})
