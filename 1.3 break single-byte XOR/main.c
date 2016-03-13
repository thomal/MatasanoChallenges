#include "cryptopals_common.h"

int main (int argc, char** argv) {
  //getFrequenciesOfFile(std::string("englishCorpus").c_str(), true);
  
  if (argc != 2) {
    printf("Usage: %s <hexstring>\n", argv[0]);
    exit(0);
  }
  
  size_t len;
  byte* str = hexStrToByteStr(argv[1], &len);
  
  byte* plaintext = breakSingleByteXOR(str, len, byteFrequencies_englishText);
  free(plaintext);
  free(str);
  return 0;
}
