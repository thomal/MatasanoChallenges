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

bool aes_ecb_detect (const byte* input, size_t in, bool log) {
  return count_duplicate_blocks(input, in, log, 16) > 0;
}

size_t count_duplicate_blocks (const byte* input, size_t in, bool log, size_t block_size) {
  using namespace std;
  auto blocks = vector<byte*>();
  size_t duplicatedBlocks = 0;
  
  //For each block in the input
  for (size_t i = 0; i < in; i+= block_size) {
    const byte* newBlock = input+i;
    bool alreadySeen = false;    
    //For each block already seen
    for (size_t j = 0; (j < blocks.size()); j++) {
      const byte* oldBlock = blocks[j];
      bool equalToOldBlock = true;
      //For each byte in the old block
      for (size_t k = 0; (k < block_size); k++) {
        if (newBlock[k] != oldBlock[k])
          equalToOldBlock = false;
      }
      if (equalToOldBlock) {
        alreadySeen = true;
        goto endSearch;
      }
    }
    
    endSearch:
    if(alreadySeen)
      duplicatedBlocks++;
    else
      blocks.push_back((byte*)memcpy((byte*)malloc(sizeof(byte)*block_size),input+i,block_size));
  }
  for (size_t i = 0; i < blocks.size(); i++)
    free(blocks[i]);
  if(log){PAD printf("%zu duplicate blocks.\n", duplicatedBlocks);}
  return duplicatedBlocks;
}
