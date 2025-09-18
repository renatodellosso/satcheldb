#pragma once

#include <unordered_map>

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
    const char* str;
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

typedef std::unordered_map<const char*, QueryEntry> Query;

struct IndexedQueryEntry {
  QueryEntryType type;
  Value value;
  int index;
};

typedef std::vector<IndexedQueryEntry> IndexedQuery;

bool areQueriesEqual(Schema schema, Query q1, Query q2);

enum UpdateEntryType {
  UPDATE_SET
};

struct UpdateEntry {
  UpdateEntryType type;
  Value value;
};

typedef std::unordered_map<const char*, UpdateEntry> Update;

struct IndexedUpdateEntry {
  UpdateEntryType type;
  Value value;
  int index;
};

typedef std::vector<IndexedUpdateEntry> IndexedUpdate;

bool areUpdatesEqual(Schema schema, Update u1, Update u2);