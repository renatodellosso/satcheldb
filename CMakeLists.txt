cmake_minimum_required(VERSION 3.15...4.0)
project(SatchelDb VERSION 0.1
                  DESCRIPTION "A small DB"
                  LANGUAGES CXX)
add_executable(SatchelDb src/main.cpp)

find_package(Catch2 3 REQUIRED)
add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)