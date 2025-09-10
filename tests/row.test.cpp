#include <catch2/catch_test_macros.hpp>
#include "../src/row.h"

TEST_CASE("returns correct row","[extractData]") {
  UnsanitizedRow unsanitized = {
    new TypedValue[3] {
      intVal(1), floatVal(2.0f), stringVal("test")
    },
    3
  };
  
  Row sanitized = new Value[3] {
    unsanitized.cols[0].data,
    unsanitized.cols[1].data,
    unsanitized.cols[2].data
  };

  Row extracted = extractData(unsanitized);
  
  REQUIRE(extracted[0].i == sanitized[0].i);
  REQUIRE(extracted[1].f == sanitized[1].f);
  REQUIRE(extracted[2].str == sanitized[2].str);
}