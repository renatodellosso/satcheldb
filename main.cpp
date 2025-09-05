#include <iostream>
#include "src/schema.h"

int main(void) {
  Schema schema = {
    1, 
    new ValueType[0], 
    std::unordered_map<char*, int>()
  };
  bool valid = isValidSchema(schema);
  std::cout << "Hello, World!" << std::endl;
  return 0;
}