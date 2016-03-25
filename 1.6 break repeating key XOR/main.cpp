#include "cryptopals_common.h"
#include "parse_options.h"

int main (int argc, char** argv) {
  optionParsing(argc, argv);
  
  byte* output = repeatingKeyXor(input, inputn, key, keyn);
  
  for (size_t i = 0; i < inputn; i++)
    printf(outputRaw?"%c":"%02x", output[i]);
  printf("\n");
  
  free(output);
  cleanupArgs();
  return 0;
}
