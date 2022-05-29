#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "table.h"

// Input
extern const char* ifile_name;
extern const char* delim;
// Output
extern const char* ofile_name;
// Defaults
const Border DEFAULT_VERTICAL_BORDER = {'|'};
const Border DEFAULT_HORIZONTAL_BORDER = {'-'};
constexpr char DEFAULT_CORNER = '+';
// Rules
extern std::vector<const char*> rules;
// Table Pseudo-struct
extern std::vector<std::vector<Cell>> cells;
extern std::vector<size_t> col_lens;
extern std::vector<std::vector<const Border*>> vborders;
extern std::vector<std::vector<const Border*>> hborders;
extern std::vector<std::vector<char>> corners;
