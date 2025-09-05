cmake_minimum_required(VERSION 3.15...4.0)
project(SatchelDb VERSION 0.1
                  DESCRIPTION "A small DB"
                  LANGUAGES CXX)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(SatchelDb main.cpp ${SOURCES})

find_package(Catch2 3 REQUIRED)
add_executable(tests tests/test.cpp ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)