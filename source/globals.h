#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include "table.h"

// Input
extern char* csv_file;
extern char* delim;
// Output
extern FILE* output;
// Defaults
extern Border DEFAULT_VERTICAL_BORDER;
extern Border DEFAULT_HORIZONTAL_BORDER;
extern char DEFAULT_CORNER;
// Table Pseudo-struct
extern std::vector<Column> columns;
extern std::vector<Border> vborders;
extern std::vector<Border> hborders;
extern std::vector<std::vector<char>> corners;
