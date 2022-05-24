#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include "errno.h"
#include "globals.h"
#include "parse.h"
#include "io.h"
#include "format.h"

using namespace std;

signed main(int argc, char* argv[]){
	try{
		parse(argc, argv);
		if(not ifile_name){
			throw noInputFileError;
		}
		readTable();
		initTable();
		writeTable();
	}catch(Error e){
		fprintf(stderr, "Error: %s\n", e.what.c_str());
		return errno;
	}

	return 0;
}
