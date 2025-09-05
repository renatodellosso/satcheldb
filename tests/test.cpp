#include <catch2/catch_test_macros.hpp>
#include "../src/schema.h"

TEST_CASE("isValidSchema", "[schema]") {
  std::unordered_map<char*, int> nameToIndex;
  char* key = new char[3] {
    'i', 'd', '\0'
  };
  nameToIndex[key] = 0;

  Schema schema = {
    1,
    new ValueType[1] {
      VT_INT
    },
    nameToIndex
  };

  REQUIRE(isValidSchema(schema) == true);
}