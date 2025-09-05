#include "row.h"

Row extractData(UnsanitizedRow unsanitized) {
  Row sanitized = new Value[unsanitized.len];
  for (int i = 0; i < unsanitized.len; i++) {
    sanitized[i] = unsanitized.cols[i].data;
  }
}