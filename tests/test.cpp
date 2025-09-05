#include <catch2/catch_test_macros.hpp>
#include "../src/schema.h"

TEST_CASE("isValidSchema", "[schema]") {
  std::unordered_map<char*, int> nameToIndex;
  nameToIndex[(char*)"id"] = 0;

  Schema schema = {
    1,
    new ValueType[1] {
      VT_INT
    },
    nameToIndex
  };

  REQUIRE(isValidSchema(schema) == true);
}