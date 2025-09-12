#pragma once

#include "table.h"
#include <unordered_map>

class MemoryTable : public Table {
  public:
    MemoryTable(Schema schema);
    virtual Row findOne(Query query);
    virtual std::vector<Row> findMany(Query query);
    virtual long size();
  protected:
    virtual bool isIdTaken(int id);
    virtual bool insertRaw(Row row);
    virtual Row getRaw(int id);
  private:
    std::unordered_map<int, Row> rows;
    bool rowMatchesQuery(Row row, QueryWithIndices query);
    bool entryMatchesRow(Row row, QueryEntryWithIndex entry);
};