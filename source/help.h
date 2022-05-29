#pragma once

const char HELP_MSG[] = R"DEL(
	tbt ([options])		: generates Text Based Table
		-h			: print help and exit
		-i [file]	: set input csv to [file]
		-o [file]	: set output to [file]; if not specified use stdout
		-d [char]	: set the delimeter the input csv uses
		-r [rule]+	: specify a semicolon separated list of [rule]s to be processed
		-f [file] 	: specify file from which new line separated rules to be read

		[rule]:
			[scope][command]+

		[scope]:
			[pos]x[pos]
			>[pos] is either a number,
			  2 numbers separted by a '-',
			 '$' signaling "last" or
			 'n' meaning all (0-$)

		[command]:
			<   : align text left
			=   : center text
			>   : align text right
			A	: write with upper case
			V	: write with lower case
			c	: write with camel case
			n	: write with orignal casing
			H[char]   : set char to use as left upper corner
			J[char]   : set char to use as right upper corner
			K[char]   : set char to use as left lower corner
			L[char]   : set char to use as right lower corner
			C[char]   : set char to use ass every corner
			h[border_cmd]   : apply [border_cmd] to left border
			j[border_cmd]   : apply [border_cmd] to bottom border
			k[border_cmd]   : apply [border_cmd] to top border
			l[border_cmd]   : apply [border_cmd] to right border
			b[border_cmd]   : apply [border_cmd] to all 4 borders
			>NOTE: direction specifiers are vim-like
			[border_cmd]:
				n       : normal
				d       : dotted
				i       : invisible
				:[char] : set char to use for drawing

		Rules are interpreted in the order of specification,
		overlapping rules override eachother.
)DEL";
