#pragma once

enum ValueType {
  VT_INT,
  VT_FLOAT,
  VT_STRING,
};

typedef
  union {
    int i;
    float f;
    char* str;
  } 
Value;

struct TypedValue {
  ValueType type;
  Value data;
};

typedef Value* Row;
struct UnsanitizedRow {
  TypedValue* cols;
  int len;
};

/**
 * Converts an unsanitized row into a row
 */
Row extractData(UnsanitizedRow unsanitized);

TypedValue value(ValueType type, Value data);
TypedValue intVal(int i);
TypedValue floatVal(float f);
TypedValue stringVal(const char* s);