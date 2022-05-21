#include "globals.h"

#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "table.h"

// Input
const char* ifile_name;
const char* delim = ";";
// Output
const char* ofile_name = NULL;
// Defaults
//Border DEFAULT_VERTICAL_BORDER;
//Border DEFAULT_HORIZONTAL_BORDER;
//char DEFAULT_CORNER;
// Rules
char* rules;
// Table Pseudo-struct
std::vector<std::vector<Cell>> cells;
std::vector<size_t> col_lens;
std::vector<size_t> inc_values;
std::vector<std::vector<const Border*>> vborders;
std::vector<std::vector<const Border*>> hborders;
std::vector<std::vector<char>> corners;
size_t inc = 0;
