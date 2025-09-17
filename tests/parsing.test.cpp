#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "../src/parsing.h"
#include "utils.h"

Schema schema = {
  3,
  new ValueType[3]{
    VT_INT, VT_FLOAT, VT_STRING 
  },
  std::unordered_map<const char *, int> {
    {(const char*)"id", 0},
    {(const char*)"num", 1},
    {(const char*)"name", 2}
  },
  new const char*[3] {
    "id", "num", "name"
  }
};

TEST_CASE("parseRow works properly") {
  simdjson::ondemand::parser parser;
  auto json = R"({
    "id": 1,
    "num": 2.0,
    "name": "test"
  })"_padded;

  simdjson::ondemand::document obj = parser.iterate(json);
  Row row = new Value[schema.colCount];
  REQUIRE(parseRow(schema, &obj, &row));

  UnsanitizedRow expected = getTestRow(1, 2.0f, "test");

  REQUIRE(areRowsEqual(schema, row, expected));

  json = R"({
    "id": 1,
    "num": 3.0,
    "name": "test"
  })"_padded;
  // obj = parser.iterate(json); // Segfaults
  BENCHMARK("parseRow") {
    return parseRow(schema, &obj, &row);
  };
}