#ifndef TABLE_H
#define TABLE_H

#include "row.h"
#include "schema.h"
#include <vector>
#include <unordered_map>

class Table {
  public:
    Table(Schema schema);
    bool isValidRow(UnsanitizedRow row);
    bool insert(UnsanitizedRow row);
    Row get(int id);
  protected:
    virtual bool isIdTaken(int id);
    virtual bool insertRaw(Row row);
    virtual Row getRaw(int id);
  private:
    Schema schema;
};

#endif