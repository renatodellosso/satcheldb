cmake_minimum_required(VERSION 3.15...4.0)
set (CMAKE_CXX_STANDARD 11)
project(SatchelDb VERSION 0.1
                  DESCRIPTION "A small DB"
                  LANGUAGES CXX)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(SatchelDb main.cpp ${SOURCES})

file(GLOB_RECURSE TESTS "tests/*.cpp" "tests/*.h")

find_package(Catch2 3 REQUIRED)
add_executable(tests ${TESTS} ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)