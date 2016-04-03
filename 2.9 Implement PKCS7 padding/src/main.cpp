#include "cryptopals_common.h"
#include "parse_options.h"

byte* withInputData (const byte* input, size_t in, const byte* input2, size_t i2n, const char* mode, bool log, size_t* outn) {
  assert(i2n == 1);
  uint8_t blocksize = input2[0];
  bool add_padding = strcmp(mode, "add") == 0;
  
  if (log) {
    printf("Input = 0x");
    for (size_t i = 0; i < in; i++)
      printf("%02x", input[i]);
    printf("\n");
    printf("Input length = %zu\n", in);
    printf("Blocksize = %" PRIu8 "\n", blocksize);
    printf("Logging = %s\n", log?"true":"false");
    printf("Mode = %s\n", mode);
  }
  
  return (add_padding) ? PKCS7_add(input, in, blocksize, outn):
                         PKCS7_remove(input, in, blocksize, outn);
}

int main (int argc, char** argv) {
  assert(sizeof(char)==sizeof(byte));
  optionParsing(argc, argv, &withInputData);
  cleanupArgs();
  return 0;
}
