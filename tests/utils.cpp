#include "utils.h"

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
};