#ifndef MEMORY_TABLE_H
#define MEMORY_TABLE_H

#include "table.h"
#include <unordered_map>

class MemoryTable : public Table {
  public:
    bool MemoryTable::insert(UnsanitizedRow row);
  protected:
    virtual bool isIdTaken(int id);
    virtual bool insertRaw(Row row);
    virtual Row getRaw(int id);
  private:
    std::unordered_map<int, Row> rows;
};

#endif