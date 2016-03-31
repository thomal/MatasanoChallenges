#include "cryptopals_common.h"
#include "parse_options.h"

byte* withInputData (const byte* input, size_t in, const byte* key, size_t kn, bool log, const char* mode, size_t* outn) {
  if (log) {
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
  }
  
  byte* output;
  *outn = in; //No padding, in must be a multiple of 16.
  if (strcmp(mode, "encrypt") == 0) {
    output = aes_ecb_encrypt(input, in, key, kn, log);
  } else if (strcmp(mode, "decrypt") == 0) {
    output = aes_ecb_decrypt(input, in, key, kn, log);
  } else {
    printf("Invalid mode %s.\n", mode);
    exit(0);
  }
  
  return output;
}

int main (int argc, char** argv) {
  assert(sizeof(const char)==sizeof(byte));
  optionParsing(argc, argv, &withInputData);
  cleanupArgs();
  return 0;
}
