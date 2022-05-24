#include "format.h"

#include <iostream>

#include <algorithm>
#include <ctype.h>
#include <string.h>
#include "globals.h"
#include "errno.h"

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
	 * while formatting cells
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
	/* Split rules and apply each
	*/
	constexpr char rdelim[] = ";";
	char* r = strtok(rules, rdelim);
	while(r != NULL){
		try{
			runRule(r);
		}catch(Warning w){
			fprintf(stderr, "Warning: %s\n", w.what.c_str());
		}
		r = strtok(NULL, rdelim);
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

void formatBorder(Border* &b, const char &c,
					const border_mode &m = border_mode::NORMAL){
	if(b == &DEFAULT_VERTICAL_BORDER || b == &DEFAULT_HORIZONTAL_BORDER){
		b = new Border;
	}else{
		delete b;
		b = new Border;
	}
	b->c = c;
	b->mode = m;
}

void runRule(std::string rule){
	/* Determine scope and command string
	*/
	int x = rule.find('x');
	if(x == std::string::npos){ throw (Warning){BAD_RULE_WARNING, BAD_RULE " ('x' missing from scope)"}; }
	int rangeLine = std::stoi(rule.substr(0, x));
	auto notDigit = [](const char& c) -> bool { return not isdigit(c); };
	int endOfScope = find_if(rule.begin() + (x+1), rule.end(), notDigit) - rule.begin();
	int rangeCol = std::stoi(rule.substr(x+1, endOfScope - (x+1)));
	rule = rule.substr(endOfScope);
	if(rule == ""){ throw (Warning){BAD_RULE_WARNING, BAD_RULE " (No command supplied)"}; }

	/* Boundary check
	*/
	if(rangeLine < 1 || rangeCol < 1 ||
			cells.size() < rangeLine || cells[0].size() < rangeCol){
		std::string msg("Rule contained a cell that was out of the bounds of the table.");
		msg += " (" + std::to_string(rangeLine) + ";" + std::to_string(rangeCol) + ")";
		throw (Warning){RULE_OF_RANGE_WARNING, msg};
	}

	/* Lower the values to represent indexes
	*/
	--rangeLine;
	--rangeCol;

	/* Set inner cell inner states acording to format
	*/
	{
		const Border* &leftBorder = vborders[rangeLine][rangeCol];
		const Border* &rightBorder = vborders[rangeLine][rangeCol + 1];
		const Border* &topBorder = hborders[rangeLine][rangeCol];
		const Border* &bottomBorder = hborders[rangeLine + 1][rangeCol];
		Border* b = new Border;
		int i = 0;
		border_mode m = border_mode::NORMAL;
		auto setBorderMode = [&](const Border* &b){
					char* c = nullptr;
					bool changed = false;
					if(i + 1 == rule.size()){ throw (Warning){BAD_RULE_WARNING, BAD_RULE " (end of command reached after 'b')"}; }
					++i;
					switch(rule[i]){
						case 'n':
							m = border_mode::NORMAL;
							changed = true;
						break;
						case 'd':
							m = border_mode::DOTTED;
							changed = true;
						break;
						case 'i':
							m = border_mode::INVISIBLE;
							changed = true;
						break;
						case ':':
							if(i + 1 == rule.size()){ throw (Warning){BAD_RULE_WARNING, BAD_RULE " (end of command reached after ':')"}; }
							++i;
							c = new char(rule[i]);
							changed = true;
						default:
							if(not changed){
								throw (Warning){BAD_RULE_WARNING, BAD_RULE " (invalid char after 'b')"};
							}else{ break; }
					}
					if(c == nullptr){
						c = new char(b->c);
					}
					formatBorder(const_cast<Border*&>(b), *c, m);
					delete c;
				};
		while(i < rule.size()){
			switch(rule[i]){
				case '<':
					cells[rangeLine][rangeCol].align = text_align::LEFT;
					break;
				case '=':
					cells[rangeLine][rangeCol].align = text_align::CENTER;
					break;
				case '>':
					cells[rangeLine][rangeCol].align = text_align::RIGHT;
					break;
				case 'b':
					{
						int h = i;
						setBorderMode(leftBorder);
						i = h;
						setBorderMode(rightBorder);
						i = h;
						setBorderMode(topBorder);
						i = h;
						setBorderMode(bottomBorder);
						i = h;
					break;
					}
				case 'h':
					setBorderMode(leftBorder);
					break;
				case 'j':
					setBorderMode(bottomBorder);
					break;
				case 'l':
					setBorderMode(topBorder);
					break;
				case 'k':
					setBorderMode(rightBorder);
					break;
			}
			i++;
		}
	}
	formatCell(cells[rangeLine][rangeCol], col_lens[rangeCol]);
}
