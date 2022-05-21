#include "io.h"

#include <string>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "errno.h"
#include "table.h"

void readTable(){
	FILE* input = fopen(ifile_name, "r");
	if(not input){
		throw inputError;
	}
	char* line = NULL;
	size_t j = 0;
	while(getline(&line, &j, input) != -1){
		cells.push_back(std::vector<Cell>());
		char* k = strtok(line, delim);
		while(k != NULL){
			cells.back().push_back( {k} );
			k = strtok(NULL, delim);
		}
		auto &last_alias = cells.back().back().str;
		last_alias = last_alias.substr(0, last_alias.size() - 1);	// remove last \n
	}

	fclose(input);
}

void writeTable(){
	FILE* output;
	if(ofile_name){
		output = fopen(ofile_name, "w");
		if(not output){ throw outputError; }
	}else{
		output = stdout;
	}
	for(int i = 0; i < cells.size(); i++){
		/* Print border lines
		*/
		for(int h = 0; h < cells[i].size(); h++){
			fputc(corners[i][h], output);
			fputs(hborders[i][h]->render(col_lens[h]).c_str(), output);
		}
		fputc(corners[i].back(), output);
		fputc('\n', output);
		/* Print data lines
		*/
		for(int h = 0; h < cells[i].size(); h++){
			fputs(vborders[i][h]->render(1).c_str(), output);
			fprintf(output, "%s", cells[i][h].rendered_str.c_str());
		}
		fputs(vborders[i].back()->render(1).c_str(), output);
		fputc('\n', output);
	}
	/* Print last (closing) border line
	*/
	for(int h = 0; h < cells.back().size(); h++){
		fputc(corners.back()[h], output);
		fputs(hborders.back()[h]->render(col_lens[h]).c_str(), output);
	}
	fputc(corners.back().back(), output);
	fputc('\n', output);

	if(output != stdout){
		fclose(output);
	}
}
