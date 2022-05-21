#pragma once

#include <algorithm>
#include <ctype.h>
#include <string.h>
#include "globals.h"

void initTable();
void formatCell(Cell &c, const size_t &width);
void formatBorders(const size_t &cellx, const size_t &celly);
void runRule(const char* rule);
