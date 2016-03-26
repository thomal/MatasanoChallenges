#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H
#include "cryptopals_common.h"
// optionParsing converts all input formats into a byte* and converts the byte*
//   returned by *withInputData into the correct output format. The return value
//   of *withInputData is free()'d.
void optionParsing(int argc, char** argv, byte* (*withInputData) (const byte* input, size_t in, const byte* key, size_t kn, bool log, const char* mode, size_t* outn));
void cleanupArgs();
#endif
