#pragma once

const char HELP_MSG[] = R"DEL(
	tbt ([options])		: generates Text Based Table
		-h			: print help and exit
		-i [file]	: set input csv to [file]
		-o [file]	: set output to [file]; if not specified use stdout
		-d [char]	: set the delimeter the input csv uses
		-r [rule]+	: specify a semicolon separated list of [rule]s to be processed
)DEL";
