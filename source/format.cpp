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
	/* Format all cells
	*/
	for(int i = 0; i < cells.size(); i++){
		for(int h = 0; h < cells[0].size(); h++){
			formatCell(cells[i][h], col_lens[h]);
		}
	}
	/* Fill borders and corners with defaults
	*/
	for(int i = 0; i < cells.size() + 1; i++){	// Horizontal borders
		hborders.push_back(std::vector<const Border*>());
		for(int h = 0; h < cells[0].size(); h++){
			hborders[i].push_back(&DEFAULT_HORIZONTAL_BORDER);
		}
	}
	for(int i = 0; i < cells.size(); i++){		// Vertical borders
		vborders.push_back(std::vector<const Border*>());
		for(int h = 0; h < cells[0].size() + 1; h++){
			vborders[i].push_back(&DEFAULT_VERTICAL_BORDER);
		}
	}
	for(int i = 0; i < cells.size() + 1; i++){	// Corners
		corners.push_back(std::vector<char>());
		for(int h = 0; h < cells[0].size() + 1; h++){
			corners.back().push_back(DEFAULT_CORNER);
		}
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
	c.rendered_str = c.str;
	switch(c.format){
		case text_format::NORMAL:
			break;
		case text_format::LOWER:
			std::transform(c.str.begin(), c.str.end(), c.rendered_str.begin(), tolower);
			break;
		case text_format::UPPER:
			std::transform(c.str.begin(), c.str.end(), c.rendered_str.begin(), toupper);
			break;
		case text_format::CAMEL:
			for(int i = 0; i < c.rendered_str.size() - 1; i++){	// trimming guarantees this doesnt overflow
				if(c.str[i] == ' '){
					c.rendered_str[i + 1] = toupper(c.str[i + 1]);
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

int getScope(const std::string &rule, int &hs, int &he, int &vs, int &ve){
	auto strToPos = [](const std::string str, const bool &line) -> int {
		if(str == "$"){
			if(line){
				return cells[0].size() + 1;
			}else{
				return cells.size() + 1;
			}
		}else{
			return std::stoi(str);
		}
	};
	try{
		int x;
		int ptr = 0;
		// ptr = 0 -> x
		while(isdigit(rule[ptr]) || rule[ptr] == '-' || rule[ptr] == '$'){
			++ptr;
		}
		if(rule[ptr] == 'x'){
			x = ptr;
			int h = rule.find('-');
			if(h != std::string::npos){
				hs = strToPos(rule.substr(0, h), true);
				he = strToPos(rule.substr(h + 1, x - h - 1), true) + 1;	// + 1: so its inclusive
			}else{
				hs = strToPos(rule.substr(0, x), true);
				he = hs + 1;
			}
		}else if(rule[ptr] == 'n'){
			hs = 1;
			he = cells.size() + 1;
			if(not(ptr + 1 >= rule.size()) && (++ptr, rule[ptr] == 'x')){
				x = ptr;
			}else{
				goto NONSENSE_SCOPE;
			}
		}else{
			NONSENSE_SCOPE:
			throw (Warning){BAD_RULE_WARNING, BAD_RULE " (non-sensical scope)"};
		}
		++ptr;
		// ptr = x -> scope_end
		while(isdigit(rule[ptr]) || rule[ptr] == '-' || rule[ptr] == '$'){
			++ptr;
		}
		if(rule[ptr] == 'n'){
			vs = 1;
			ve = cells[0].size() + 1;
			++ptr;
		}else{
			int h = rule.find('-', x);
			if(h != std::string::npos){
				vs = strToPos(rule.substr(0, h), false);
				ve = strToPos(rule.substr(h + 1, x - h - 1), false) + 1;	// + 1: so its inclusive
			}else{
				vs = strToPos(rule.substr(x + 1, ptr - x), false);
				ve = vs + 1;
			}
		}
		return ptr;
	}catch(std::invalid_argument){
		throw (Warning){BAD_RULE_WARNING, BAD_RULE " (specified scope is not a number)"};
	}
}

void runRule(std::string rule){
	if(rule == ""){ throw (Warning){BAD_RULE_WARNING, BAD_RULE " (No command supplied)"}; }

	/* Determine scope and cut $rule to command string
	*/
	int hs, he, 
		vs, ve;
	rule = rule.substr(getScope(rule, hs, he, vs, ve));

	/* Sanity check
	*/
	if(hs > he || vs > ve){
		throw (Warning){BAD_RULE_RANGE_WARNING, "(negative range specified)"};
	}
	if(hs == he || vs == ve){
		throw (Warning){BAD_RULE_RANGE_WARNING, "(null range specified)"};
	}

	/* Convert user supplied 1 indexed positions to 0 indexed ones
	*/
	--hs;
	--vs;
	--he;
	--ve;

	/* Boundary check
	*/
	if(hs < 0 || vs < 0 ||
			cells.size() < he || cells[0].size() < ve){
		throw (Warning){BAD_RULE_RANGE_WARNING, "Rule contained a cell that was out of the bounds of the table."};
	}

	/* Set inner cell inner state he
s acording to format
	*/
	for(int line = hs; line < he; line++){
		for(int col = vs; col < ve; col++){
			/* Aliasing for sanitys sake */
			const Border* &leftBorder = vborders[line][col];
			const Border* &rightBorder = vborders[line][col + 1];
			const Border* &topBorder = hborders[line][col];
			const Border* &bottomBorder = hborders[line + 1][col];
			char &ULCorner = corners[line][col];
			char &URCorner = corners[line][col + 1];
			char &LLCorner = corners[line + 1][col];
			char &LRCorner = corners[line + 1][col + 1];
			Cell &cell = cells[line][col];
			/**/
			Border* b = new Border;
			int i = 0;
			border_mode m = border_mode::NORMAL;
			auto setCorner = [&](char &c){
						if(i + 1 == rule.size()){ throw (Warning){BAD_RULE_WARNING, BAD_RULE " (end of command reached after corner specifier)"}; }
						++i;
						c = rule[i];
			};
			auto setBorderMode = [&](const Border* &b){
						char* c = nullptr;
						bool changed = false;
						if(i + 1 == rule.size()){ throw (Warning){BAD_RULE_WARNING, BAD_RULE " (end of command reached after border specifier)"}; }
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
									throw (Warning){BAD_RULE_WARNING, BAD_RULE " (invalid char after border specifier)"};
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
						cell.align = text_align::LEFT;
						break;
					case '=':
						cell.align = text_align::CENTER;
						break;
					case '>':
						cell.align = text_align::RIGHT;
						break;
					case 'A':
						cell.format = text_format::UPPER;
						break;
					case 'V':
						cell.format = text_format::LOWER;
						break;
					case 'c':
						cell.format = text_format::CAMEL;
						break;
					case 'n':
						cell.format = text_format::NORMAL;
						break;
					case 'H':
						setCorner(ULCorner);
						break;
					case 'J':
						setCorner(URCorner);
						break;
					case 'K':
						setCorner(LLCorner);
						break;
					case 'L':
						setCorner(LRCorner);
						break;
					case 'C':
						setCorner(ULCorner);
						setCorner(URCorner);
						setCorner(LLCorner);
						setCorner(LRCorner);
						break;
					case 'b':
						{
							int j, h = i;
							setBorderMode(leftBorder);
							j = i;
							i = h;
							setBorderMode(rightBorder);
							i = h;
							setBorderMode(topBorder);
							i = h;
							setBorderMode(bottomBorder);
							i = j;
						}
						break;
					case 'h':
						setBorderMode(leftBorder);
						break;
					case 'j':
						setBorderMode(bottomBorder);
						break;
					case 'k':
						setBorderMode(topBorder);
						break;
					case 'l':
						setBorderMode(rightBorder);
						break;
					default:
						std::string msg = BAD_RULE " (unrecognized char '";
						msg += rule[i];
						msg += "' in rule)";
						throw (Warning){BAD_RULE_WARNING, msg.c_str()};
				}
				i++;
			}
			formatCell(cell, col_lens[col]);
		}
	}
}
