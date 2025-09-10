#include "table.h"
#include <vector>

Table::Table(Schema schema) {
  if (!isValidSchema(schema))
    throw std::invalid_argument("Schema is invalid.");

  this->schema = schema;
}

// #define DEBUG_IS_VALID_ROW_CAUSES

bool Table::isValidRow(UnsanitizedRow row) {
  // IDs must be >=0
  if (row.len < 1 || row.cols[0].data.i < 0) {
#ifdef DEBUG_IS_VALID_ROW_CAUSES
    throw std::invalid_argument("Invalid ID");
#else
    return false;
#endif
  }

  if (this->schema.colCount != row.len) {
#ifdef DEBUG_IS_VALID_ROW_CAUSES
    throw std::invalid_argument("Wrong colCount");
#else
    return false;
#endif
  }

  for (int i = 0; i < this->schema.colCount; i++) {
    if (row.cols[i].type != this->schema.colTypes[i]) {
#ifdef DEBUG_IS_VALID_ROW_CAUSES
    throw std::invalid_argument("Wrong column type");
#else
    return false;
#endif
    }
  }

  return true;
}

bool Table::insert(UnsanitizedRow row) {
  if (!isValidRow(row))
    return false;

  Row toInsert = extractData(row);

  return insertRaw(toInsert);
}

Row Table::get(int id) {
  if (id < 0)
    return NULL;

  return getRaw(id);
}
