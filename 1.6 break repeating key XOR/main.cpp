#include "cryptopals_common.h"
#include "parse_options.h"

void decrypt (const byte* input, size_t inputn, bool log = false) {
  byte* output = breakRepeatingKeyXor(input, inputn, log);
  
  if (output == nullptr) {
    if (log) {printf("Failed to decrypt.\n");}
    return;
  }
  
  if (log){printf("Plaintext:\n");}
  for (size_t i = 0; i < inputn; i++) {
    if (outputF == RAW) {
      printf("%c", output[i]);
    } else {
      assert(outputF == HEX); //ensured by optionParsing()
      printf("%02x", output[i]);
    }
  }
  if(log){printf("\n");}
  
done:
  free(output);
}

void tokensFromHexFile (const char* hexData) {
  size_t inputn;
  byte* input = hexStrToByteStr(hexData, &inputn);
  decrypt(input, inputn);
  free(input);
}

int main (int argc, char** argv) {
  assert(sizeof(const char)==sizeof(byte));
  optionParsing(argc, argv);
  
  if (inputF == HEXFILE_TOKENS) {
    withTokens((const char*)input, &tokensFromHexFile);
  } else {
    assert(inputF == RAW); //HEX and B64FILE are silently converted to RAW
    decrypt(input, inputn, true);
  }
  
  cleanupArgs();
  return 0;
}
