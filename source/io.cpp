#include "io.h"

#include <string>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "table.h"

bool readTable(){
	FILE* f = fopen(csv_file, "r");
	if(f == NULL){ return 1; }

	char* line = NULL;
	size_t j = 0;
	while(getline(&line, &j, f) != -1){
		Column temp_col;
		char* k = strtok(line, delim);
		while(k != NULL){
			Cell cell = { k };
			temp_col.cells.push_back(cell);
			k = strtok(NULL, delim);
		}
		std::string &last_alias = temp_v[temp_v.size() - 1];
		last_alias = last_alias.substr(0, last_alias.size() - 1);	// remove last \n
		cells.push_back(temp_v);
	}

	fclose(f);

	return 0;
}

bool writeTable(){
	for(int i = 0; i < columns.size(); i++){
		for(int h = 0; h < columns[i].cells.size(); h++){
			fprintf(output, "%s ", columns[i].cells.str[h].c_str());
		}
		fputs("\n", output);
	}
	

	return 0;
}
