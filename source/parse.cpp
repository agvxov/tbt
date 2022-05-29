#include "parse.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "globals.h"
#include "help.h"
#include "errno.h"
#include "io.h"

void parse(int argc, char* argv[]){
	opterr = 0;	// gnu getopts variable; by setting it to 0 we state that we want to handle our errors by ourselfs

	char c;
	while((c = getopt(argc, argv, "ho:i:d:r:f:")) != -1){
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
				{
					constexpr char rdelim[] = ";";
					rules.emplace_back(strtok(optarg, rdelim));
					while(rules.back() != NULL){
						rules.emplace_back(strtok(NULL, rdelim));
					}
					rules.pop_back();
				}
				break;
			case 'f':
				readRules(optarg);
				break;
			default:
				throw unknownArgumentError;
		}
	}
}
