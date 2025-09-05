#include "table.h"
#include <vector>

Table::Table(int colCount, ValueType* colSchema) {
  this->colCount = colCount;
  this->colSchema = colSchema;
}

bool Table::validateRow(UnsanitizedRow row) {
  if (this->colCount != row.len)
    return false;

  for (int i = 0; i < this->colCount; i++) {
    if (row.cols[i].type != this->colSchema[i])
      return false;
  }

  return true;
}
