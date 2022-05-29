#pragma once

#include <vector>

enum class border_mode{
	NORMAL,
	DOTTED,
	INVISIBLE
};

enum class text_align{
	LEFT,
	CENTER,
	RIGHT
};

enum class text_format{
	NORMAL,
	LOWER,
	UPPER,
	CAMEL
};

struct Border{
	char c;
	border_mode mode = border_mode::NORMAL;
	std::string render(const size_t &width) const {
		std::string r(1, this->c);
		for(int i = 1; i < width; i++){
			r += r[0];
		}
		switch(this->mode){
			case border_mode::NORMAL:
				break;
			case border_mode::DOTTED:
				for(int i = 0; i < width / 2; i++){
					r[(i*2) + 1] = ' ';
				}
				break;
			case border_mode::INVISIBLE:
				for(int i = 0; i < width; i++){
					r[i] = ' ';
				}
				break;
		}
		return r;
	}
};

struct Cell{
	std::string str;
	std::string rendered_str;
	text_align align = text_align::LEFT;
	text_format format = text_format::NORMAL;
};

