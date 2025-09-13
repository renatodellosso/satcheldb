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

void applyUpdateEntry(Row row, IndexedUpdateEntry entry) {
  switch (entry.type)
  {
  case UPDATE_SET:
    row[entry.index] = entry.value;
    break;
  
  default:
    throw std::invalid_argument("Unknown entry type");
  }
}

void applyUpdate(Row row, IndexedUpdate update) {
  for (auto ptr : update) {
    applyUpdateEntry(row, ptr);
  }
}

bool Table::updateOne(Query query, Update update) {
  Row row = findOne(query);
  if (row == NULL)
    return false;

  IndexedUpdate indexedUpdate = addIndicesToUpdate(update);

  applyUpdate(row, indexedUpdate);
  return true;
}

void moveIdToFrontOfQuery(IndexedQuery* query) {
  for (int i = 0; i < query->size(); i++) {
    if ((*query)[0].index == 0) {
      IndexedQueryEntry entry = (*query)[i];
      query->erase(query->begin() + i);
      query->insert(query->begin(), entry);
      break;
    }
  }
}

IndexedQuery Table::addIndicesToQuery(Query query) {
  IndexedQuery indexedQuery;

  for (auto ptr = query.begin(); ptr != query.end(); ptr++) {
    IndexedQueryEntry indexedEntry = addIndicesToQueryEntry(ptr->first, ptr->second);
    if (indexedEntry.index == -1)
      throw std::invalid_argument("Invalid query");
    indexedQuery.push_back(indexedEntry);
  }

  moveIdToFrontOfQuery(&indexedQuery);
  return indexedQuery;
}

IndexedQueryEntry Table::addIndicesToQueryEntry(char* key, QueryEntry entry) {
    if (schema.nameToIndex.find(key) == schema.nameToIndex.end()) {
      return {
        QUERY_EQ, NULL, -1
      };
    }

    int index = schema.nameToIndex[key];
    IndexedQueryEntry indexedEntry = {
      entry.type, entry.value, index
    };
    return indexedEntry;
}

void moveIdToFrontOfUpdate(IndexedUpdate* update) {
  for (int i = 0; i < update->size(); i++) {
    if ((*update)[0].index == 0) {
      IndexedUpdateEntry entry = (*update)[i];
      update->erase(update->begin() + i);
      update->insert(update->begin(), entry);
      break;
    }
  }
}

IndexedUpdate Table::addIndicesToUpdate(Update update) {
  IndexedUpdate indexedUpdate;

  for (auto ptr = update.begin(); ptr != update.end(); ptr++) {
    IndexedUpdateEntry indexedEntry = addIndicesToUpdateEntry(ptr->first, ptr->second);
    if (indexedEntry.index == -1)
      throw std::invalid_argument("Invalid update");
    indexedUpdate.push_back(indexedEntry);
  }

  moveIdToFrontOfUpdate(&indexedUpdate);
  return indexedUpdate;
}

IndexedUpdateEntry Table::addIndicesToUpdateEntry(char* key, UpdateEntry entry) {
  if (schema.nameToIndex.find(key) == schema.nameToIndex.end()) {
    return {
      UPDATE_SET, NULL, -1
    };
  }

  int index = schema.nameToIndex[key];
  IndexedUpdateEntry indexedEntry = {
    entry.type, entry.value, index
  };
  return indexedEntry;
}

Schema Table::getSchema() {
  return schema;
}