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
  Row row;
  // REQUIRE(parseRow(schema, &obj, &row));

  // UnsanitizedRow expected = getTestRow(1, 2.0f, "test");

  // REQUIRE(areRowsEqual(schema, row, expected));

  BENCHMARK("parseRow") {
    json = R"({
      "id": 1,
      "num": 3.0,
      "name": "test"
    })"_padded;
    obj = parser.iterate(json);
    return parseRow(schema, &obj, &row);
  };
}

TEST_CASE("parse string") {
  simdjson::ondemand::parser parser;
  auto json = R"({
    "name": "test"
  })"_padded;
  simdjson::ondemand::document obj = parser.iterate(json);
  json = R"({
    "name": "test"
  })"_padded;
  obj = parser.iterate(json);
  simdjson::ondemand::document* ptr = &obj;

  simdjson::ondemand::value objVal;
  auto error = (*ptr)["name"].get(objVal);

  if (error)
    throw new std::invalid_argument("error");

  std::string_view valSv(objVal);

  char* str = new char[valSv.length() + 1];

  for (int i = 0; i < valSv.length(); i++)
    str[i] = valSv.at(i);
  str[valSv.length()] = '\0';

  REQUIRE(std::strcmp(str, "test") == 0);
}