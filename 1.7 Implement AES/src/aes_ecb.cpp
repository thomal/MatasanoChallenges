#include "cryptopals_common.h"

byte* aes_ecb(const byte* input, size_t in, const byte* key, size_t kn, bool log, bool encrypt) {
  assert((in%16) == 0);
  size_t blocks = in/16;
  byte* processed = (byte*)malloc(sizeof(byte)*16*blocks);
  
  indent++;
  for (size_t block = 0; block < blocks; block++) {
    if (log) {PAD printf("Block %zu/%zu\n", block+1, blocks);}
    indent++;
    byte* processedBlock;
    if (encrypt)
      processedBlock = aes_encrypt_block(input+(block*16), key, kn, log);
    else
      processedBlock = aes_decrypt_block(input+(block*16), key, kn, log);
    memcpy(processed+(block*16), processedBlock, 16);
    free(processedBlock);
    indent--;
  }
  indent--;
  
  return processed;
}

byte* aes_ecb_encrypt(const byte* input, size_t in, const byte* key, size_t kn, bool log) {
  return aes_ecb(input, in, key, kn, log, true);
}

byte* aes_ecb_decrypt(const byte* input, size_t in, const byte* key, size_t kn, bool log) {
  return aes_ecb(input, in, key, kn, log, false);
}
