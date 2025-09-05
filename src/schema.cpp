#include "schema.h"

bool isValidSchema(Schema schema) {
  if (schema.colCount == 0)
    return false;

  // Check for ID col
  if (schema.colTypes[0] != VT_INT)
    return false;
  if (schema.nameToIndex["id"] != 0)
    return false;
}