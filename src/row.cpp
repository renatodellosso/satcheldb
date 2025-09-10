#include "row.h"

Row extractData(UnsanitizedRow unsanitized) {
  Row sanitized = new Value[unsanitized.len];
  for (int i = 0; i < unsanitized.len; i++) {
    sanitized[i] = unsanitized.cols[i].data;
  }
  return sanitized;
}

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
  return value(VT_FLOAT, val);
}

TypedValue stringVal(const char* s) {
  Value val;
  val.str = (char*)s;
  return value(VT_STRING, val);
}