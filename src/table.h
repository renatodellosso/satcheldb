#pragma once

#include "row.h"
#include "schema.h"
#include <vector>
#include <unordered_map>

class Table {
  public:
    Table(Schema schema);
    bool isValidRow(UnsanitizedRow row);
    // -1 ID indicates an ID should be auto-selected
    bool insert(UnsanitizedRow row);
    Row get(int id);
    virtual Row findOne(Query query) = 0;
    virtual std::vector<Row> findMany(Query query) = 0;
    bool updateOne(Query query, Update update);
    int updateMany(Query query, Update update);
    virtual Row deleteOne(Query query) = 0;
    virtual std::vector<Row> deleteMany(Query query) = 0;
    virtual long size() = 0;
    Schema getSchema();
  protected:
    virtual bool isIdTaken(int id) = 0;
    virtual bool insertRaw(Row row) = 0;
    virtual Row getRaw(int id) = 0;
    IndexedQuery addIndicesToQuery(Query query);
    IndexedQueryEntry addIndicesToQueryEntry(const char* key, QueryEntry entry);
    IndexedUpdate addIndicesToUpdate(Update update);
    IndexedUpdateEntry addIndicesToUpdateEntry(const char* key, UpdateEntry entry);
    Schema schema;
};