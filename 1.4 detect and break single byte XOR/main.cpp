#include "cryptopals_common.h"

void findSingleByteXOR (const char* line) {
  size_t len;
  byte* str = hexStrToByteStr(line, &len);
  byte* plaintext = breakSingleByteXOR(str, len, byteFrequencies_englishText);
  if (plaintext != nullptr) {
    for (size_t i = 0; i < len; i++)
      printf("%c", plaintext[i]);
    printf("\n");
    free(plaintext);
  }
  free(str);
}

int main (int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    exit(0);
  }
  
  withFile(argv[1], &findSingleByteXOR);
  
  return 0;
}
