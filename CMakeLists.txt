cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(pong VERSION 1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR
    src
)
set(INC_DIR inc)
set(BUILD_DIR build)

file(GLOB SOURCES 
    ${SRC_DIR}/*.c
    ${SRC_DIR}/game/*.c
    ${SRC_DIR}/ui/*.c
)
#file(GLOB HEADERS ${INC_DIR}/*.h)

find_package(SDL2 REQUIRED)

add_executable(pong ${SOURCES})

include_directories(${INC_DIR} ${SDL2_INCLUDE_DIRS})

target_link_libraries(pong ${SDL2_LIBRARIES})

set_target_properties(pong PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BUILD_DIR})
