#include "cryptopals_common.h"

byte* PKCS7(const byte* input, size_t in, uint8_t blocksize, size_t* outn) {
  uint8_t toAdd = 0;
  while (((in+toAdd)%blocksize) != 0)
    toAdd++;
  *outn = in+toAdd;
  byte* output = (byte*) malloc(sizeof(byte)**outn);
  memcpy(output, input, in);
  for (size_t i = 0; i < toAdd; i++)
    (output+in)[i] = toAdd;
  return output;
}
