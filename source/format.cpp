#include "tbt_format.h"

#include "tbt_globals.h"

void formatTable(){
	for(int i = 0; i < columns.size(); i++){
		for(int h = 0; h < columns[i].cells.size(); h++){
			corners.push_back(DEFAULT_CORDER);
		}
	}
}
