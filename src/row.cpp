#include "row.h"
#include <string>
#include "schema.h"

Row extractData(UnsanitizedRow unsanitized) {
  Row sanitized = new Value[unsanitized.len];
  for (int i = 0; i < unsanitized.len; i++) {
    sanitized[i] = unsanitized.cols[i].data;
  }
  return sanitized;
}

bool areRowsEqual(Schema schema, Row row, UnsanitizedRow other) {
  if (row == NULL)
    return false;
  if (schema.colCount != other.len)
    return false;

  for (int i = 0; i < schema.colCount; i++) {
    if (schema.colTypes[i] != other.cols[i].type || !areValuesEqual(schema.colTypes[i], row[i], other.cols[i].data))
      return false;
  }

  return true;
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

bool areValuesEqual(ValueType type, Value v1, Value v2) {
  switch (type)
  {
  case VT_INT:
    return v1.i == v2.i;
  case VT_FLOAT:
    return v1.f == v2.f;
  case VT_STRING:
    return std::strcmp(v1.str, v2.str) == 0;
  default:
    return false;
  }
}

bool areQueriesEqual(Schema schema, Query q1, Query q2) {
  if (q1.size() != q2.size())
    return false;

  for (auto entry : q1) {
    auto other = q2.find(entry.first);
    if (other == q2.end())
      return false; // Not found
    
    if (entry.second.type != other->second.type)
      return false;
    
    auto index = schema.nameToIndex.find(entry.first);
    if (index == schema.nameToIndex.end())
      return false; // Invalid key

    if (!areValuesEqual(schema.colTypes[index->second], entry.second.value, other->second.value));
  }

  return true;
}

bool areUpdatesEqual(Schema schema, Update u1, Update u2) {
  if (u1.size() != u2.size())
    return false;

  for (auto entry : u1) {
    auto other = u2.find(entry.first);
    if (other == u2.end())
      return false; // Not found
    
    if (entry.second.type != other->second.type)
      return false;
    
    auto index = schema.nameToIndex.find(entry.first);
    if (index == schema.nameToIndex.end())
      return false; // Invalid key

    if (!areValuesEqual(schema.colTypes[index->second], entry.second.value, other->second.value));
  }

  return true;
}