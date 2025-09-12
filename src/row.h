#pragma once

#include <unordered_map>
// #include "schema.h"

struct Schema; // Forward declare to handle circular includes

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
bool areRowsEqual(Schema schema, Row row, UnsanitizedRow other);

TypedValue value(ValueType type, Value data);
TypedValue intVal(int i);
TypedValue floatVal(float f);
TypedValue stringVal(const char* s);
bool areValuesEqual(ValueType type, Value v1, Value v2);

enum QueryEntryType {
  QUERY_EQ
};

struct QueryEntry {
  QueryEntryType type;
  Value value;
};

typedef std::unordered_map<char*, QueryEntry> Query;

struct QueryEntryWithIndex {
  QueryEntryType type;
  Value value;
  int index;
};

typedef std::vector<QueryEntryWithIndex> QueryWithIndices;