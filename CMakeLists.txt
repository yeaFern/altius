cmake_minimum_required(VERSION 2.8.12)
project(altius)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SRC_LIST LIST_DIRECTORIES true
    src/*.h
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
