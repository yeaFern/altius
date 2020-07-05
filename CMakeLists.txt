cmake_minimum_required(VERSION 2.8.12)
project(altius)

file(GLOB_RECURSE SRC_LIST
    src/*.h
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME})
