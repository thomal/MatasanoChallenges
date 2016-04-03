#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H
#include "cryptopals_common.h"
// optionParsing converts all input formats into a byte*
void optionParsing(int argc, char** argv, byte* (*withInputData) (const byte* input, size_t in, const byte* input2, size_t i2n, const char* mode, bool log, size_t* outn));
void cleanupArgs();
#endif
