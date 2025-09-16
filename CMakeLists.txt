cmake_minimum_required(VERSION 3.15...4.0)
set (CMAKE_CXX_STANDARD 17)
project(SatchelDb VERSION 0.1
                  DESCRIPTION "A small DB"
                  LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  simdjson
  GIT_REPOSITORY https://github.com/simdjson/simdjson.git
  GIT_TAG  tags/v3.6.0
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(simdjson)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(SatchelDb main.cpp ${SOURCES})
target_link_libraries(SatchelDb PRIVATE simdjson::simdjson)

file(GLOB_RECURSE TESTS "tests/*.cpp" "tests/*.h")

find_package(Catch2 3 REQUIRED)
add_executable(tests ${TESTS} ${SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE simdjson::simdjson)