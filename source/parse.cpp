#include "parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "globals.h"
#include "help.h"
#include "errno.h"

void parse(int argc, char* argv[]){
	opterr = 0;	// gnu getopts variable; by setting it to 0 we state that we want to handle our errors by ourselfs

	char c;
	while((c = getopt(argc, argv, "ho:i:d:r:")) != -1){
		switch(c){
			case 'h':
				puts(HELP_MSG);
				exit(0);
			case 'o':
				ofile_name = optarg;
				break;
			case 'i':
				ifile_name = optarg;
				break;
			case 'd':
				delim = optarg;
				break;
			case 'r':
				rules = optarg;
				break;
			default:
				throw unknownArgumentError;
		}
	}
}
