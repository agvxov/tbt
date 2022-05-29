#pragma once

#define _GNU_SOURCE

#include <string>
#include <stdio.h>
#include <string.h>
#include "globals.h"

void readTable();
void readRules(const char* file);
void writeTable();
