#include <catch2/catch_test_macros.hpp>
#include "../src/schema.h"


TEST_CASE("returns true for valid schema", "[isValidSchema]") {
  std::unordered_map<const char*, int> nameToIndex;
  nameToIndex[(char*)"id"] = 0;

  Schema schema = {
    1,
    new ValueType[1] {
      VT_INT
    },
    nameToIndex
  };

  REQUIRE(isValidSchema(schema));
}

TEST_CASE("returns false for schema with 0 column count", "[isValidSchema]") {
  std::unordered_map<const char*, int> nameToIndex;
  nameToIndex[(char*)"id"] = 0;

  Schema schema = {
    0,
    new ValueType[1] {
      VT_INT
    },
    nameToIndex
  };

  REQUIRE(!isValidSchema(schema));
}

TEST_CASE("returns false for schema with an ID column of the wrong type", "[isValidSchema]") {
  std::unordered_map<const char*, int> nameToIndex;
  nameToIndex[(char*)"id"] = 0;

  Schema schema = {
    0,
    new ValueType[1] {
      VT_FLOAT
    },
    nameToIndex
  };

  REQUIRE(!isValidSchema(schema));

  schema.colTypes[0] = VT_STRING;
  REQUIRE(!isValidSchema(schema));
}

TEST_CASE("returns false for schema with an ID column in the wrong index", "[isValidSchema]") {
  std::unordered_map<const char*, int> nameToIndex;
  nameToIndex[(char*)"id"] = 1;

  Schema schema = {
    0,
    new ValueType[1] {
      VT_FLOAT
    },
    nameToIndex
  };

  REQUIRE(!isValidSchema(schema));

  schema.nameToIndex = std::unordered_map<const char*, int>(); // Remove col name
  schema.colTypes[0] = VT_INT;
  REQUIRE(!isValidSchema(schema));
}