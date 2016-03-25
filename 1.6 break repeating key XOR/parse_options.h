#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H
#include "cryptopals_common.h"
// Interface
// -k hex/-k raw  determines how key is read
// -i hex/-p raw  determines how input is read
// -o hex/-o raw  determines how output is printed
// -x "password"
// -y "The message/plaintext goes here."

// Variables
extern bool keyRaw, inputRaw, outputRaw;
extern byte *key, *input;
extern size_t keyn, inputn;

//Functions
void optionParsing(int argc, char** argv);
void cleanupArgs();
#endif
