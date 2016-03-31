#include "cryptopals_common.h"
#include "parse_options.h"

byte* withInputData (const byte* input, size_t in, const byte* input2, size_t i2n, bool log, size_t* outn) {
  assert(i2n == 1);
  uint8_t blocksize = input2[0];
  
  if (log) {
    printf("Input = 0x");
    for (size_t i = 0; i < in; i++)
      printf("%02x", input[i]);
    printf("\n");
    printf("Input length = %zu\n", in);
    printf("Blocksize = %" PRIu8 "\n", blocksize);
    printf("Logging = %s\n", log?"true":"false");
  }
  
  return PKCS7(input, in, blocksize, outn);
}

int main (int argc, char** argv) {
  assert(sizeof(char)==sizeof(byte));
  optionParsing(argc, argv, &withInputData);
  cleanupArgs();
  return 0;
}
