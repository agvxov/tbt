#include "format.h"

#include <algorithm>
#include <ctype.h>
#include <string.h>
#include "globals.h"

void initTable(){
	/* Force every line to be the same lenght by finding the longest and
	 * appending empty cells where needed
	*/
	size_t max_cells = 0;
	for(int i = 0; i < cells.size(); i++){
		if(cells[i].size() > max_cells){
			max_cells = cells[i].size();
		}
	}
	for(int i = 0; i < cells.size(); i++){
		while(cells[i].size() < max_cells){
			cells[i].push_back( {""} );
		}
	}
	/* Trim all sorounding whitespaces from cells and
	 * padd with a single space on both sides
	*/
	for(int i = 0; i < cells.size(); i++){
		for(int h = 0; h < cells[i].size(); h++){
			constexpr char whiteSpace[] = " \t\n";
			std::string &str_alias = cells[i][h].str;
			str_alias.erase(str_alias.find_last_not_of(whiteSpace) + 1);
			str_alias.erase(0, str_alias.find_first_not_of(whiteSpace));
			str_alias = ' ' + str_alias + ' ';
		}
	}
	/* Get column widths by finding the widest string in each
	*/
	for(int i = 0; i < cells[0].size(); i++){
		size_t max = 0;
		for(int h = 0; h < cells.size(); h++){
			if(cells[h][i].str.size() > max){
				max = cells[h][i].str.size();
			}
		}
		col_lens.push_back(max);
	}
	/* Fill corners and borders with defaults
	 * while formatting cells to default
	*/
	for(int i = 0; i < cells.size(); i++){
		corners.push_back(std::vector<char>());
		vborders.push_back(std::vector<const Border*>());
		hborders.push_back(std::vector<const Border*>());
		for(int h = 0; h < cells[i].size(); h++){
			corners.back().push_back(DEFAULT_CORNER);
			vborders[i].push_back(&DEFAULT_VERTICAL_BORDER);
			hborders[i].push_back(&DEFAULT_HORIZONTAL_BORDER);
			formatCell(cells[i][h], col_lens[h]);
		}
		corners.back().push_back(DEFAULT_CORNER);
		vborders[i].push_back(&DEFAULT_VERTICAL_BORDER);
	}
}

void formatCell(Cell &c, const size_t &width){
	if(inc){
		switch(c.inc_mode){
			case text_incement::DEC:
				c.rendered_str = std::to_string(c.inc_val);
				break;
			case text_incement::HEX:
				//c.rendered_str = c.inc_val;
				break;
			case text_incement::ROMAN:
				//c.rendered_str = to_roman(c.inc_val);
				break;
		}
	}else{
		c.rendered_str = c.str;
	}
	switch(c.format){
		case text_format::NORMAL:
			break;
		case text_format::LOWER:
			std::transform(c.str.begin(), c.str.end(), c.str.begin(), tolower);
			break;
		case text_format::UPPER:
			std::transform(c.str.begin(), c.str.end(), c.str.begin(), toupper);
			break;
		case text_format::CAMEL:
			for(int i = 0; i < c.rendered_str.size(); i++){	// trimming guarantees this doesnt overflow
				if(c.str[i] == ' '){
					c.str[i + 1] = toupper(c.str[i + 1]);
				}
			}
			break;
	}
	switch(c.align){
		case text_align::LEFT:
			while(c.rendered_str.size() < width){
				c.rendered_str = c.rendered_str + ' ';
			}
			break;
		case text_align::CENTER:	// add shifting to the other side when imperfect width
			{
			const int i = (width - c.rendered_str.size()) / 2;
			for(int h = 0; h < i; h++){
				c.rendered_str = ' ' + c.rendered_str;
			}
			}
			while(c.rendered_str.size() < width){
				c.rendered_str = c.rendered_str + ' ';
			}
			break;
		case text_align::RIGHT:
			while(c.rendered_str.size() < width){
				c.rendered_str = ' ' + c.rendered_str;
			}
			break;
	}
}

void runRule(const char* rule){

}
