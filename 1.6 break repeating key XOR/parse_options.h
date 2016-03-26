#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H
#include "cryptopals_common.h"
// Interface
// -i [hex|raw|hexfile_tokens|b64file]  determines how input is read
// -o [hex|raw]  determines how output is printed
// -y "The input data goes here."
// Hex: lowercase (automatically converted to RAW by optionParsing(), null bytes are fine)
// Raw: raw bytes (cannot contain null bytes or inputn is invalid)
// Hexfile Tokens: A file of whitespace seperated hexstrings, each to be
//   processed independantly as if invoked with -i hex -y <token>
// b64file: Base 64 file, base64 data, possibly spread over multiple lines with
//   other meaningless whitespace. Disregard all non-base64 characters and parse
//   as one block of data. automatically converted to RAW by optionParsing(),
//   null bytes are fine.

// Variables
typedef enum {UNDEFINED, RAW, HEX, HEXFILE_TOKENS, B64FILE} IOFormat;

extern IOFormat inputF, outputF;
extern byte *input;
extern size_t inputn;

//Functions
void optionParsing(int argc, char** argv);
void cleanupArgs();
#endif
