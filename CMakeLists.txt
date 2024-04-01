cmake_minimum_required(VERSION 3.10)
project(QmkFormatConverter C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -pedantic -std=gnu17 -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SUBDIRS
    ${PROJECT_SOURCE_DIR}/src/logger
    ${PROJECT_SOURCE_DIR}/src/QJTCP
)

set(INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/src/logger"
    "${PROJECT_SOURCE_DIR}/src/QJTCP"
)

set(LIBS 
    "QfcLogger"
    "QJTCP"
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/logger)
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/QJTCP)

add_executable(QmkFormatConverter src/main.c)
target_link_libraries(QmkFormatConverter PUBLIC ${LIBS})
target_include_directories(QmkFormatConverter PUBLIC ${SUBDIRS})

# Unit Tests
add_executable(Test test/test.c)
target_link_libraries(Test PUBLIC ${LIBS})
target_include_directories(Test PUBLIC ${SUBDIRS})

enable_testing()
add_test(NAME Tests COMMAND Test)

add_custom_command(
     TARGET Test
     COMMENT "Run tests"
     POST_BUILD 
     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
     COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> -R "^Test$" --output-on-failures
)

