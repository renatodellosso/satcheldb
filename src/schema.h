#pragma once
#include "row.h"
#include <unordered_map>

struct Schema {
  int colCount;
  ValueType* colTypes;
  std::unordered_map<const char*, int> nameToIndex;
  const char** colNames;
};

bool isValidSchema(Schema schema);