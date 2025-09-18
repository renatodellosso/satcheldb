#pragma once

#include "row.h"
#include "schema.h"
#include "simdjson.h"

bool parseRow(Schema schema, simdjson::ondemand::document* doc, Row* row);
bool parseQuery(Schema schema, simdjson::ondemand::document* doc, Query* query);
bool parseUpdate(Schema schema, simdjson::ondemand::document* doc, Update* update);
void convertRowToJson(Schema schema, Row row, std::string* out);