#pragma once

#include <stdlib.h>
#include <string>

enum Problem_id{
	NO_INPUT_FILE_ERROR,
	INPUT_ERROR,
	OUTPUT_ERROR,
	UNKNOWN_ARGUMENT_ERROR,
	RULE_OF_RANGE_WARNING,
	BAD_RULE_WARNING
};

class Problem{
	public:
		int num;
		std::string what;
};

class Error : public Problem{};
class Warning : public Problem{};

extern const Error noInputFileError;
extern const Error inputError;
extern const Error outputError;
extern const Error unknownArgumentError;
//extern const Warning ruleOutOfRange;
#define BAD_RULE "Malformed rule."
