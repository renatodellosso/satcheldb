#pragma once

#include "row.h"
#include "schema.h"
#include "simdjson.h"

bool parseRow(Schema schema, simdjson::ondemand::document* doc, Row* row);
simdjson::ondemand::document convertRowToJson(Schema schema, Row row);
Query parseQuery(simdjson::ondemand::document doc);
Update parseUpdate(simdjson::ondemand::document doc);