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