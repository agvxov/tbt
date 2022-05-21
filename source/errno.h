#pragma once

struct Error{
	int num;
	const char* what; 
};

extern const Error noInputFileError;
extern const Error inputError;
extern const Error outputError;
extern const Error unknownArgumentError;
