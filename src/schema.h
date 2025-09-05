#pragma once
#include "row.h"
#include <unordered_map>

struct Schema {
  int colCount;
  ValueType* colTypes;
  std::unordered_map<char*, int> nameToIndex;
};

bool isValidSchema(Schema schema);