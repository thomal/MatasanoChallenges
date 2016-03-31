#include "cryptopals_common.h"
#include "parse_options.h"

void withInputData (const byte* input, size_t in, bool log) {
  static size_t inputNum = 0;
  inputNum++; //Numbering starts at 1
  
  if (aes_ecb_detect(input, in, log)) {
    printf("%zu: Likely enciphered using AES in ECB mode.\n", inputNum);
    printf("%zu: Input (pretty):\n", inputNum);
    for (size_t i = 0; i < in; i++)
      printf("%02x%s", input[i], (((i+1)%16)==0)?"\n":" ");
    printf("%zu: Input: 0x\n", inputNum);
    for (size_t i = 0; i < in; i++)
      printf("%02x", input[i]);
    printf("\n");
    printf("Input length = %zu\n", in);
  } else if (log) {
    printf("%zu: Couldn't detect cipher.\n", inputNum);
  }
  
}

int main (int argc, char** argv) {
  assert(sizeof(char)==sizeof(byte));
  optionParsing(argc, argv, &withInputData);
  cleanupArgs();
  return 0;
}
