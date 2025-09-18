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

TEST_CASE("parseQuery works properly") {
  simdjson::ondemand::parser parser;
  auto json = R"({
    "id": 1,
    "num": 2.0,
    "name": "test"
  })"_padded;

  simdjson::ondemand::document obj = parser.iterate(json);
  Query query;
  REQUIRE(parseQuery(schema, &obj, &query));

  Query expected = {
    {
      "id",
      {
        QUERY_EQ,
        {
          .i = 1
        }
      }
    },
    {
      "num",
      {
        QUERY_EQ,
        {
          .f = 2.0f
        }
      }
    },
    {
      "name",
      {
        QUERY_EQ,
        {
          .str = "test"
        }
      }
    },
  };

  REQUIRE(areQueriesEqual(schema, query, expected));

  json = R"({
    "id": 1,
    "num": 3.0,
    "name": "test"
  })"_padded;
  // obj = parser.iterate(json); // Segfaults
  BENCHMARK("parseQuery") {
    return parseQuery(schema, &obj, &query);
  };
}

TEST_CASE("parseUpdate works properly") {
  simdjson::ondemand::parser parser;
  auto json = R"({
    "id": 1,
    "num": 2.0,
    "name": "test"
  })"_padded;

  simdjson::ondemand::document obj = parser.iterate(json);
  Update update;
  REQUIRE(parseUpdate(schema, &obj, &update));

  Update expected = {
    {
      "id",
      {
        UPDATE_SET,
        {
          .i = 1
        }
      }
    },
    {
      "num",
      {
        UPDATE_SET,
        {
          .f = 2.0f
        }
      }
    },
    {
      "name",
      {
        UPDATE_SET,
        {
          .str = "test"
        }
      }
    },
  };

  REQUIRE(areUpdatesEqual(schema, update, expected));

  json = R"({
    "id": 1,
    "num": 3.0,
    "name": "test"
  })"_padded;
  // obj = parser.iterate(json); // Segfaults
  BENCHMARK("parseUpdate") {
    return parseUpdate(schema, &obj, &update);
  };
}