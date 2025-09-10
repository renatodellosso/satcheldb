#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <unordered_map>
#include "../src/memoryTable.h"

MemoryTable getTable() {
  return MemoryTable({
    3,
    new ValueType[3]{VT_INT, VT_FLOAT, VT_STRING},
    std::unordered_map<char *, int> {
      {(char*)"id", 0},
      {(char*)"num", 1},
      {(char*)"name", 2}
    }
  });
}

/**
 * Call outside of benchmarks to avoid increasing time
 */
UnsanitizedRow getTestRow(int id, float data, const char* msg) {
  return {
    new TypedValue[3] {
      intVal(id),
      floatVal(data),
      stringVal(msg)
    },
    3,
  };
}

TEST_CASE("insert and get work with valid input") {
  MemoryTable table = getTable();

  bool insertResult = table.insert(getTestRow(99, 1.23f, "Hello World"));
  REQUIRE(insertResult);

  UnsanitizedRow row = getTestRow(100, 1.23f, "Hello World");
  BENCHMARK("insert valid") {
    return table.insert(row);
  };

  Row getResult = table.get(99);
  REQUIRE(getResult[0].i == 99);
  REQUIRE(getResult[1].f == 1.23f);
  REQUIRE(std::strcmp(getResult[2].str, "Hello World") == 0);

  BENCHMARK("get existing") {
    return table.get(100);
  };
}

TEST_CASE("insert allows overwrites") {
  MemoryTable table = getTable();

  UnsanitizedRow row = getTestRow(99, 1.23f, "Hello World");
  table.insert(row);

  bool insertResult = table.insert(getTestRow(99, 1.23f, "Hello World 2"));
  REQUIRE(insertResult);

  Row got = table.get(99);
  REQUIRE(std::strcmp(got[2].str, "Hello World 2") == 0);

  BENCHMARK("insert already-existing") {
    return table.insert(row);
  };
}

TEST_CASE("insert fails with invalid ID") {
  MemoryTable table = getTable();

  REQUIRE(!table.insert(getTestRow(-1, 1.23f, "Hello World")));

  UnsanitizedRow row = getTestRow(-1, 1.23f, "Hello World");
  BENCHMARK("insert invalid id") {
    return table.insert(row);
  };
}

TEST_CASE("insert fails for wrong # of columns") {
  MemoryTable table = getTable();

  REQUIRE(!table.insert({
    new TypedValue[2] {
      intVal(102),
      floatVal(1.23f),
    },
    2,
  }));

  UnsanitizedRow row = {
    new TypedValue[2] {
      intVal(102),
      floatVal(1.23f),
    },
    2,
  };
  BENCHMARK("insert wrong col count") {
    return table.insert(row);
  };
}

TEST_CASE("get returns NULL for non-existing row") {
  MemoryTable table = getTable();

  REQUIRE(table.get(1) == NULL);

  BENCHMARK("get non-existing") {
    return table.get(1);
  };

}

TEST_CASE("get returns NULL for invalid ID") {
  MemoryTable table = getTable();

  REQUIRE(table.get(-1) == NULL);

  BENCHMARK("get invalid ID") {
    return table.get(-1);
  };
}