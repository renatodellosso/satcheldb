#include "table.h"
#include <vector>

Table::Table(Schema schema) {
  if (!isValidSchema(schema))
    throw std::invalid_argument("Schema is invalid.");

  this->schema = schema;
}

bool Table::isValidRow(UnsanitizedRow row) {
  if (this->schema.colCount != row.len)
    return false;

  for (int i = 0; i < this->schema.colCount; i++) {
    if (row.cols[i].type != this->schema.colTypes[i])
      return false;
  }

  // IDs must be >=0
  if (row.cols[0].data.i < 0)
    return false;

  return true;
}

bool Table::insert(UnsanitizedRow row) {
  if (!isValidRow(row))
    return false;

  Row toInsert = extractData(row);

  if (isIdTaken(toInsert[0].i))
    return false;

  return insertRaw(toInsert);
}

Row Table::get(int id) {
  if (id < 0)
    return NULL;

  return getRaw(id);
}
