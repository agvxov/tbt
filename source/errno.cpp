#include "errno.h"

#include <stdlib.h>

const Error noInputFileError = {NO_INPUT_FILE_ERROR, "Input file not specified."};
const Error inputError = {INPUT_ERROR, "Input error."};
const Error outputError = {OUTPUT_ERROR, "Output error."};
const Error unknownArgumentError = {UNKNOWN_ARGUMENT_ERROR, "Unknown argument was specified."};
//const Warning ruleOutOfRange = {RULE_OF_RANGE_WARNING, "Rule contained a cell that was out of the bounds of the table."}
