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

void moveIdToFrontOfQuery(QueryWithIndices* query) {
  for (int i = 0; i < query->size(); i++) {
    if ((*query)[0].index == 0) {
      QueryEntryWithIndex entry = (*query)[i];
      query->erase(query->begin() + i);
      query->insert(query->begin(), entry);
      break;
    }
  }
}

QueryWithIndices Table::addIndicesToQuery(Query query) {
  QueryWithIndices indexedQuery;

  for (auto ptr = query.begin(); ptr != query.end(); ptr++) {
    QueryEntryWithIndex indexedEntry = addIndicesToQueryEntry(ptr->first, ptr->second);
    if (indexedEntry.index == -1)
      throw std::invalid_argument("Invalid query");
    indexedQuery.push_back(indexedEntry);
  }

  moveIdToFrontOfQuery(&indexedQuery);
  return indexedQuery;
}

QueryEntryWithIndex Table::addIndicesToQueryEntry(char* key, QueryEntry entry) {
    if (schema.nameToIndex.find(key) == schema.nameToIndex.end()) {
      return {
        QUERY_EQ, NULL, -1
      };
    }

    int index = schema.nameToIndex[key];
    QueryEntryWithIndex indexedEntry = {
      entry.type, entry.value, index
    };
    return indexedEntry;
}