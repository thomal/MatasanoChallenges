#include "cryptopals_common.h"
#include "parse_options.h"

byte* withInputData (const byte* input, size_t in, const byte* key, size_t kn, bool log, const char* mode, size_t* outn) {
  printf("Input = 0x");
  for (size_t i = 0; i < in; i++)
    printf("%02x", input[i]);
  printf("\n");
  printf("Input length = %zu\n", in);
  
  printf("Key = 0x");
  for (size_t i = 0; i < kn; i++)
    printf("%02x", key[i]);
  printf("\n");
  printf("Key length = %zu\n", kn);
  
  printf("Logging = %s\n", log?"true":"false");
  printf("Mode = %s\n", mode);
  
  *outn = in;
  byte* output = (byte*)malloc(sizeof(byte)*in);
  memcpy(output, input, in);
  return output;
}

int main (int argc, char** argv) {
  assert(sizeof(const char)==sizeof(byte));
  optionParsing(argc, argv, &withInputData);
  cleanupArgs();
  return 0;
}
