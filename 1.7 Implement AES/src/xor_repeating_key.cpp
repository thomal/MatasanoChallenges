#include "cryptopals_common.h"

byte* repeatingKeyXor (const byte* input, size_t n, const byte* key, size_t keyn) {
  byte* plaintext = (byte*) memcpy(malloc(n*sizeof(byte)), input, n);
  
  for (size_t pi = 0, ki = 0; pi < n; pi++) {
    plaintext[pi] ^= key[ki];
    if(++ki >= keyn)
      ki=0;
  }
  
  return plaintext;
}

//find keysize. (The edit distance between two blocks) divided by the keysize is
//  closest to zero when the keysize is correct.
//split into $keysize seperate ciphertexts such that every byte in a given CT
//  was encrypted with the same byte of the key. Call these blocks.
//break each message with breakSingleByteXor
//interleave messages to reconstruct plaintext
byte* breakRepeatingKeyXor (const byte* ciphertext, size_t len, bool log, size_t indentLevel, const size_t* byteFreqs, size_t minKeysize, size_t maxKeysize, double threshold) {
  byte* plaintext = nullptr;

  if (byteFreqs == nullptr)
    byteFreqs = DEFAULT_BYTE_FREQUENCIES;
  if (minKeysize < 2) {
    minKeysize = 2;
  }
  if (maxKeysize < 2) {
    maxKeysize = 0.2*len;
    if (maxKeysize > 40)
      maxKeysize = 40;
  }
  
  //Find keysize
  if (log) {PAD printf("Determining keysize:\n");}
  indentLevel++;
  double minAvgNormDist = 0;
  size_t bestKeysize = 0;
  
  for (size_t keysize = minKeysize; keysize <= maxKeysize; keysize++) {
    //find avarage edit distance between each consecutive pair of blocks
    size_t blocks = floor((double)len/keysize);
    double sumNormalizedDistances = 0;
    double countOfPairsAvaraged = 0;
    for (size_t block = 0; block < blocks; block+=2) {
      if (block+1 != blocks) {
        size_t offset1 = (block*keysize);
        size_t offset2 = ((block+1)*keysize);
        const byte* firstBlock = ciphertext+offset1;
        const byte* secondBlock = ciphertext+offset2;
        size_t dist = editDistance(firstBlock, secondBlock, keysize);
        sumNormalizedDistances += (double)dist/keysize;
        countOfPairsAvaraged++;
      }
    }
    
    double avgNormalizedDist = sumNormalizedDistances / countOfPairsAvaraged;
    bool isDefaultReplacement = bestKeysize == 0;
    if ((avgNormalizedDist < minAvgNormDist) || isDefaultReplacement) {
      minAvgNormDist = avgNormalizedDist;
      bestKeysize = keysize;
      if(log) {
        if (isDefaultReplacement) {
          PAD printf("Initial keysize candidate:  %zu\t(%f)\n", bestKeysize, minAvgNormDist);
        } else {
          PAD printf("New best keysize candidate: %zu\t(%f)\n", bestKeysize, minAvgNormDist);
        }
      }
    }
  }
  
  //Log details about found keysize
  if (minAvgNormDist == -1) {
    PAD printf("Couldn't find keysize.");
    return nullptr;
  }
  if (log) {PAD printf("Found keysize, keysize = %zu\n", bestKeysize);}
  indentLevel--;
  
  //Deinterlace (let di be short for deinterlace, let a message formed by the
  //  process of deinterlacing be known as a deinterlaced block, or diBlock)
  if (log) {PAD printf("Deinterlacing\n");}
  indentLevel++;
  double blockCount = ceil((double)len/bestKeysize);
  double maxDiMsgSize = 1*blockCount; //1 byte per block
  byte** diBlocks = (byte**) malloc(sizeof(byte*)*bestKeysize);
  size_t* diBlockSizes = (size_t*) malloc(sizeof(size_t)*bestKeysize);
  for (size_t diBlock = 0; diBlock < bestKeysize; diBlock++) {
    diBlocks[diBlock] = (byte*) malloc(sizeof(byte)*maxDiMsgSize);
    for (size_t block = 0; block < blockCount; block++) {
      size_t index = (block*bestKeysize)+diBlock;
      if (index < len) {
        diBlocks[diBlock][block] = ciphertext[index];
        diBlockSizes[diBlock] = block+1;
      }
    }
  }
  
  indentLevel--;
  
  //Break deinterlaced blocks
  byte** diPlainBlocks = (byte**) malloc(sizeof(byte*)*bestKeysize);
  for (size_t diBlock = 0; diBlock < bestKeysize; diBlock++) {
    if (log) {PAD printf("Breaking deinterlaced block %zu of %zu\n", diBlock+1, bestKeysize);}
    diPlainBlocks[diBlock] = breakSingleByteXor(diBlocks[diBlock], diBlockSizes[diBlock], log, indentLevel+1, nullptr, threshold);
    if (!diPlainBlocks[diBlock]) {
      if(log){
        printf("WARNING - Failed to break deinterlaced block %zu, hex block follows:", diBlock+1);
        for (size_t i = 0; i < diBlockSizes[diBlock]; i++)
          printf("%02x", diBlocks[diBlock][i]);
        printf("\n");
      }
      //Cleanup
      for (size_t diBlock2 = 0; diBlock2 < diBlock; diBlock2++) {
        free(diBlocks[diBlock2]);
        if (diPlainBlocks[diBlock2] != nullptr) {
          free(diPlainBlocks[diBlock2]);
        }
      }
      free(diBlocks);
      free(diPlainBlocks);
      free(diBlockSizes);
      return plaintext; //nullptr
    }
  }
  
  //Interlace blocks
  if (log) {PAD printf("Interlacing plaintext\n");}
  plaintext = (byte*) malloc(sizeof(byte)*len);
  for (size_t i=0, diBlock=0, offsetWithinDiBlock=0; i < len; i++) {
    if (diPlainBlocks[diBlock]) {
      plaintext[i] = diPlainBlocks[diBlock][offsetWithinDiBlock];
    } else {
      plaintext[i] = 0;
    }
    if (++diBlock >= bestKeysize) {
      diBlock = 0;
      offsetWithinDiBlock++;
    }
  }
  
  //Cleanup and return (N.B.: Nearly duplicated above)
  for (size_t diBlock = 0; diBlock < bestKeysize; diBlock++) {
    free(diBlocks[diBlock]);
    if (diPlainBlocks[diBlock] != nullptr) {
      free(diPlainBlocks[diBlock]);
    }
  }
  free(diBlocks);
  free(diPlainBlocks);
  free(diBlockSizes);
  return plaintext;
}
