#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H
#include "cryptopals_common.h"
// optionParsing converts all input formats into a byte*
void optionParsing(int argc, char** argv, void (*withInputData) (const byte* input, size_t in, bool log));
void cleanupArgs();
#endif
