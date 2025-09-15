#include "../src/row.h"

/**
 * Call outside of benchmarks to avoid increasing time
 */
UnsanitizedRow getTestRow(int id, float data, const char* msg);