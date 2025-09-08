#include <catch2/catch_test_macros.hpp>
#include "../src/row.h"

TypedValue value(ValueType type, Value data) {
  return {
    type, data
  };
}

TypedValue intVal(int i) {
  Value val;
  val.i = i;

  return value(VT_INT, val);
}

TypedValue floatVal(float f) {
  Value val;
  val.f = f;
  return value(VT_INT, val);
}

TypedValue stringVal(const char* s) {
  Value val;
  val.str = (char*)s;
  return value(VT_INT, val);
}

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