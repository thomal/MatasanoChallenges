#include "cryptopals_common.h"

byte* singleByteXor (const byte* input, byte key, size_t n) {
  byte* plaintext = (byte*) memcpy(malloc(n*sizeof(byte)), input, n);
  
  for (size_t i = 0; i < n; i++)
    plaintext[i] ^= key;
  
  return plaintext;
}

double singleByteXorScore (const byte* ciphertext, byte key, size_t len, const size_t* byteFreqs) {
  byte* plaintext = singleByteXor(ciphertext, key, len);
  size_t* textFreqs = getByteFrequencies(plaintext, len);
  int score = compareByteFrequencies(textFreqs, byteFreqs);
  free(textFreqs);
  free(plaintext);
  return score;
}

byte* breakSingleByteXor (const byte* ciphertext, size_t len, bool log, const size_t* byteFreqs, double threshold) {
  if (byteFreqs == nullptr)
    byteFreqs = DEFAULT_BYTE_FREQUENCIES;
  double minScore = singleByteXorScore(ciphertext, 0, len, byteFreqs);
  byte bestKey = 0;
  
  for (int i = 1; i < 256; i++) {
    double score = singleByteXorScore(ciphertext, (byte)i, len, byteFreqs);
    if (score < minScore) {
      minScore = score;
      bestKey  = (byte)i;
    }
  }
  
  if (minScore > threshold)
    return nullptr;
  
  byte* plaintext = singleByteXor(ciphertext, bestKey, len);
  
  //Output to stdout
  if (log) {
    //PAD printf("Score: %f\n", minScore);
    PAD printf("Key: 0x%02x\n", bestKey);
    PAD printf("Key ASCII: %c\n", bestKey);
    /*PAD printf("Plaintext: ");
    for (size_t i = 0; i < len; i++)
      printf("%x", plaintext[i]);
    printf("\n");*/
    /*PAD printf("ASCII: ");
    for (size_t i = 0; i < len; i++)
      printf("%c", replaceUnprintableWithQuestion(plaintext[i]));
    printf("\n");*/
  }
  return plaintext;
}
