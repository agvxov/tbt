#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include "tbt_errno.h"
#include "tbt_globals.h"
#include "tbt_parse.h"
#include "tbt_io.h"
#include "tbt_format.h"

using namespace std;

signed main(int argc, char* argv[]){
	parse(argc, argv);
	if(readTable()){ return INPUT_ERROR; }
	formatTable();
	if(writeTable()){ return OUTPUT_ERROR; }

	return 0;
}
