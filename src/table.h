#ifndef TABLE_H
#define TABLE_H

#include "value.h"
#include <vector>

typedef Value* Row;
struct UnsanitizedRow {
  TypedValue* cols;
  int len;
};

class Table {
  public:
    Table(int colCount, ValueType* colSchema);
    bool validateRow(UnsanitizedRow row);
    virtual bool insert(UnsanitizedRow row);
  private:
    int colCount;
    ValueType* colSchema;
};

#endif