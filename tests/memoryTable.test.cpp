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

  UnsanitizedRow row1 = getTestRow(99, 1.23f, "Hello World");
  bool insertResult = table.insert(row1);
  REQUIRE(insertResult);

  UnsanitizedRow row2 = getTestRow(100, 1.23f, "Hello World");
  BENCHMARK("insert valid") {
    return table.insert(row2);
  };

  Row getResult = table.get(99);
  REQUIRE(areRowsEqual(table.getSchema(), getResult, row1));

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

TEST_CASE("findOne works with single query") {
  MemoryTable table = getTable();
  UnsanitizedRow row = getTestRow(1, 2.0f, "3");

  table.insert(row);
  table.insert(getTestRow(2, 3.0f, "4"));

  Query query = {
    { 
      (char*)"id", 
      {
        QUERY_EQ,
        {
          .i = 1
        }
      } 
    }
  };

  Row result = table.findOne(query);
  REQUIRE(areRowsEqual(table.getSchema(), result, row));

  BENCHMARK("findOne existing with single int entry") {
    return table.findOne(query);
  };

  query = {
    { 
      (char*)"num", 
      {
        QUERY_EQ,
        {
          .f = 2.0f
        }
      }
    }
  };

  result = table.findOne(query);
  REQUIRE(areRowsEqual(table.getSchema(), result, row));

  BENCHMARK("findOne existing with single float entry") {
    return table.findOne(query);
  };

  query = {
    { 
      (char*)"name", 
      {
        QUERY_EQ,
        {
          .str = (char*)"3"
        }
      }
    }
  };

  result = table.findOne(query);
  REQUIRE(areRowsEqual(table.getSchema(), result, row));

  BENCHMARK("findOne existing with single string entry") {
    return table.findOne(query);
  };

  // Benchmark larger tables
  for(int i = 100; table.size() < 1000000; i++) {
    table.insert(getTestRow(i, 3.0f + i/10.0f, "4"));
  }

  query = {
    {
      (char*)"id", 
      {
        QUERY_EQ,
        {
          .i = 1
        }
      } 
    }
  };

  BENCHMARK("findOne existing with single int entry in large table") {
    return table.findOne(query);
  };

  query = {
    { 
      (char*)"num", 
      {
        QUERY_EQ,
        {
          .f = 2.0f
        }
      }
    }
  };

  BENCHMARK("findOne existing with single float entry in large table") {
    return table.findOne(query);
  };

  query = {
    { 
      (char*)"name", 
      {
        QUERY_EQ,
        {
          .str = (char*)"3"
        }
      }
    }
  };

  BENCHMARK("findOne existing with single string entry in large table") {
    return table.findOne(query);
  };
}

TEST_CASE("findOne works with multiple-entry query") {
  MemoryTable table = getTable();
  UnsanitizedRow row = getTestRow(1, 2.0f, "3");

  table.insert(row);
  table.insert(getTestRow(2, 3.0f, "4"));

  Query query = {
    { 
      (char*)"num", 
      {
        QUERY_EQ,
        {
          .f = 2.0f
        }
      }
    },
    { 
      (char*)"name", 
      {
        QUERY_EQ,
        {
          .str = (char*)"3"
        }
      }
    }
  };

  Row result = table.findOne(query);
  REQUIRE(areRowsEqual(table.getSchema(), result, row));

  BENCHMARK("findOne existing with multiple-entry query") {
    return table.findOne(query);
  };

  // Benchmark larger tables
  for(int i = 100; table.size() < 1000000; i++) {
    table.insert(getTestRow(i, i % 5 == 0 ? 2.0f : 4.0f, "4"));
  }

  BENCHMARK("findOne existing with multiple-entry query in large table") {
    return table.findOne(query);
  };
}

TEST_CASE("findOne returns NULL for non-existing result") {
  MemoryTable table = getTable();
  UnsanitizedRow row = getTestRow(1, 2.0f, "3");

  table.insert(row);
  table.insert(getTestRow(2, 3.0f, "4"));

  Query query = {
    { 
      (char*)"num", 
      {
        QUERY_EQ,
        {
          .f = 3.0f
        }
      }
    },
    { 
      (char*)"name", 
      {
        QUERY_EQ,
        {
          .str = (char*)"3"
        }
      }
    }
  };

  Row result = table.findOne(query);
  REQUIRE(result == NULL);

  BENCHMARK("findOne non-existing with multiple-entry query") {
    return table.findOne(query);
  };

  // Benchmark larger tables
  for(int i = 100; table.size() < 1000000; i++) {
    table.insert(getTestRow(i, i % 5 == 0 ? 2.0f : 4.0f, "4"));
  }

  BENCHMARK("findOne non-existing with multiple-entry query in large table") {
    return table.findOne(query);
  };
}

TEST_CASE("findMany works with single query") {
  MemoryTable table = getTable();
  std::vector<UnsanitizedRow> rows = {
    getTestRow(1, 2.0f, "3"),
    getTestRow(2, 2.0f, "3"),
    getTestRow(3, 2.0f, "3"),
  };

  for (auto row : rows)
    table.insert(row);
  table.insert(getTestRow(4, 3.0f, "4"));

  Query query = {
    { 
      (char*)"num", 
      {
        QUERY_EQ,
        {
          .f = 2.0f
        }
      } 
    }
  };

  std::vector<Row> result = table.findMany(query);
  REQUIRE(result.size() == rows.size());
  for (int i = 0; i < result.size(); i++) {
    for (auto original : rows) {
      if (original.cols[0].data.i == result[i][0].i)
        REQUIRE(areRowsEqual(table.getSchema(), result[i], original));
    }
  }

  BENCHMARK("findMany existing with single float entry") {
    return table.findMany(query);
  };

  query = {
    { 
      (char*)"name", 
      {
        QUERY_EQ,
        {
          .str = (char*)"3"
        }
      }
    }
  };

  result = table.findMany(query);
  REQUIRE(result.size() == rows.size());
  for (int i = 0; i < result.size(); i++) {
    for (auto original : rows) {
      if (original.cols[0].data.i == result[i][0].i)
        REQUIRE(areRowsEqual(table.getSchema(), result[i], original));
    }
  }

  BENCHMARK("findMany existing with single string entry") {
    return table.findMany(query);
  };

  // Benchmark larger tables
  for(int i = 100; table.size() < 1000000; i++) {
    table.insert(getTestRow(i, 4.0f + i/10.0f, "4"));
  }

  query = {
    {
      (char*)"num", 
      {
        QUERY_EQ,
        {
          .f = 2.0f
        }
      } 
    }
  };

  BENCHMARK("findMany existing with single float entry in large table") {
    return table.findMany(query);
  };

  query = {
    { 
      (char*)"name", 
      {
        QUERY_EQ,
        {
          .str = (char*)"3"
        }
      }
    }
  };

  BENCHMARK("findMany existing with single string entry in large table") {
    return table.findMany(query);
  };
}