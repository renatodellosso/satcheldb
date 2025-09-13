#include "memoryTable.h"

MemoryTable::MemoryTable(Schema schema) : Table::Table(schema) {}

bool MemoryTable::isIdTaken(int id) {
  return rows[id];
}

bool MemoryTable::insertRaw(Row row) {
  rows[row[0].i] = row;
  return true;
}

Row MemoryTable::getRaw(int id) {
  return rows[id];
}

bool MemoryTable::entryMatchesRow(Row row, IndexedQueryEntry entry) {
  Value val = row[entry.index];

  switch (entry.type)
  {
  case QUERY_EQ:
    return areValuesEqual(schema.colTypes[entry.index], val, entry.value);
  default:
    throw std::invalid_argument("Unknown query entry type");
  }
}

bool MemoryTable::rowMatchesQuery(Row row, IndexedQuery query) {
  for (int i = 0; i < query.size(); i++) {
    if (!entryMatchesRow(row, query[i]))
      return false;
  }

  return true;
}

Row MemoryTable::findOne(Query query) {
  IndexedQuery indexedQuery = addIndicesToQuery(query);
  if (indexedQuery.size() == 0)
    return NULL;

  IndexedQueryEntry id = indexedQuery[0];
  if (id.index == 0 && id.type == QUERY_EQ) {
    Row row = rows[id.value.i];
    if (rowMatchesQuery(row, indexedQuery))
      return row;
    return NULL;
  }

  for (auto ptr = rows.begin(); ptr != rows.end(); ptr++) {
    Row row = ptr->second;

    if (rowMatchesQuery(row, indexedQuery))
      return row;
  }

  return NULL;
}

std::vector<Row> MemoryTable::findMany(Query query) {
  IndexedQuery indexedQuery = addIndicesToQuery(query);
  if (indexedQuery.size() == 0)
    return std::vector<Row>();

  IndexedQueryEntry id = indexedQuery[0];
  std::vector<Row> result;
  if (id.index == 0 && id.type == QUERY_EQ) {
    Row row = rows[id.value.i];
    if (rowMatchesQuery(row, indexedQuery)) {
      result.push_back(row);
      return result;
    }
  }

  for (auto ptr = rows.begin(); ptr != rows.end(); ptr++) {
    Row row = ptr->second;

    if (rowMatchesQuery(row, indexedQuery))
      result.push_back(row);
  }

  return result;
}

long MemoryTable::size() {
  return rows.size();
}