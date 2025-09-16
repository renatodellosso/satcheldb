#include "parsing.h"
#define SIMDJSON_DEVELOPMENT_CHECKS 1

// Use ptr instead of return to avoid memory being deleted
void stringViewToCharPtr(std::string_view sv, char** str) {
  // For some reason we have to do this manually
  *str = new char[sv.length() + 1];

  for (int i = 0; i < sv.length(); i++)
    (*str)[i] = sv.at(i);
  (*str)[sv.length()] = '\0';
}

bool parseRowEntry(ValueType type, const char* key, simdjson::ondemand::document* obj, Value* val) {
  simdjson::ondemand::value objVal;
  auto error = (*obj)[key].get(objVal);

  if (error)
    return false;

  // if (std::strcmp(key, "name") == 0)
  //   throw std::invalid_argument(key);

  switch (type)
  {
  case VT_INT: {
    *val = {
      .i = (int)objVal.get_int64()
    };
    break;
  }

  case VT_FLOAT: {
    double dbl = objVal.get_double();
    *val = {
      .f = (float)dbl
    };
    break;
  }

  case VT_STRING: {
    std::string_view valSv(objVal); // Segfaults
    // objVal.get(valSv); // Segfault here
    throw std::invalid_argument(key);

    char* str;
    stringViewToCharPtr(valSv, &str);

    *val = {
      .str = str
    };
    break;
  }
  }

  return true;
}

bool parseRow(Schema schema, simdjson::ondemand::document* doc, Row* row) {
  for (int i = 0; i < schema.colCount; i++) {
    Value val;
    if(!parseRowEntry(schema.colTypes[i], schema.colNames[i], doc, &val))
      return false;
    (*row)[i] = val;
  }

  return true;
}