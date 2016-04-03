#include "cryptopals_common.h"

byte* PKCS7_add(const byte* input, size_t in, uint8_t blocksize, size_t* outn) {
  uint8_t toAdd = 0;
  while (((in+toAdd)%blocksize) != 0)
    toAdd++;
  if (toAdd == 0)
    toAdd = blocksize;
  *outn = in+toAdd;
  byte* output = (byte*) malloc(sizeof(byte)**outn);
  memcpy(output, input, in);
  for (size_t i = 0; i < toAdd; i++)
    (output+in)[i] = toAdd;
  return output;
}

byte* PKCS7_remove(const byte* input, size_t in, uint8_t blocksize, size_t* outn) {
  uint8_t toRemove = input[in-1];
  *outn = in-toRemove;
  byte* output = (byte*) malloc(sizeof(byte)**outn);
  memcpy(output, input, *outn);
  return output;
}
