#include "tbt_globals.h"

#include <vector>
#include <string>
#include <stdio.h>
#include "table.h"

// Input
char* csv_file = "test/test.csv";
char* delim = ";";
// Output
FILE* output = stdout;
// Defaults
constexpr Border DEFAULT_VERTICAL_BORDER = {'|'};
constexpr Border DEFAULT_HORIZONTAL_BORDER = {'-'};
constexpr char DEFAULT_CORNER = '+';
// Table Pseudo-struct
std::vector<Column> columns;
std::vector<Border> vborders;
std::vector<Border> hborders;
std::vector<std::vector<char>> corners;
