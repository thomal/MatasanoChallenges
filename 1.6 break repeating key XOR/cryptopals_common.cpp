#include "cryptopals_common.h"

const size_t byteFrequencies_englishText[] =  {0,0,0,0,0,0,0,0,0,0,10661,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95154,31,890,1,60,1,31,1418,37,37,36,0,4977,1028,14508,12,294,620,322,191,165,181,142,141,143,225,21,84,0,0,0,151,1,1138,587,507,328,450,393,304,649,3262,366,57,278,482,375,542,536,6,377,837,1342,115,41,844,5,133,1,1,0,1,0,10,0,32434,6138,8638,19031,46978,7727,8690,23171,21705,596,4453,16058,9763,26780,32707,5925,226,21587,20199,36955,11645,2853,11863,567,7278,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

char base64arr[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                    'U', 'V', 'W', 'X', 'Y', 'Z',
                    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                    'u', 'v', 'w', 'x', 'y', 'z',
                    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                    '+', '/'};

int hexDigitToInt (char hexDigit) {
  if (hexDigit >= '0' && hexDigit <= '9')
    return hexDigit - '0';
  else if (hexDigit >= 'a' && hexDigit <= 'f')
    return 10 + (hexDigit - 'a');
  else if (hexDigit >= 'A' && hexDigit <= 'F')
    return 10 + (hexDigit - 'A');
  else
    printf("invalid hex digit '%c'\n", hexDigit), exit(1);
}

//Allocates new buffer to hold byte*, puts size in *n
byte* hexStrToByteStr (const char* hexstr, size_t* n) {
  *n = ceil(strlen(hexstr)/2.0);
  byte* bytestr = (byte*) calloc(*n, sizeof(byte));
  
  for (size_t ib = 0, ih = 0; ib < *n; ib++, ih+=2) {
    if (ib == 0 && ((strlen(hexstr) % 2) != 0))
      bytestr[ib] = hexDigitToInt(hexstr[ih--]);
    else
      bytestr[ib] = (hexDigitToInt(hexstr[ih])<<4) + hexDigitToInt(hexstr[ih+1]);
  }
  
  return bytestr;
}

//TODO this is horribly inefficient.
int b64CharToInt (char c) {
  for (size_t i = 0; i < 64; i++)
    if (base64arr[i] == c)
      return i;
  printf("Unknown alleged 'base64' character: '%c' (0x%x)\n", c, c);
  return -1;
}

byte* b64StrToByteStr (const char* b64str, size_t* n) {
  //From 1.1:
  /*
    //Collect relevent values locally
    i64 = (ib/3)*4; //Index of first base-64 character to set
    byte byte0 = bytestr[(bytes-1)-(ib+0)];
    byte byte1 = bytestr[(bytes-1)-(ib+1)];
    byte byte2 = bytestr[(bytes-1)-(ib+2)];
    //Get values of base-64 digits
    base64[i64+0] = A(  byte0 & 0b00111111);
    base64[i64+1] = A(((byte0 & 0b11000000) >> 6) | ((byte1 & 0b00001111) << 2));
    base64[i64+2] = A(((byte1 & 0b11110000) >> 4) | ((byte2 & 0b00000011) << 4));
    base64[i64+3] = A(((byte2 & 0b11111100) >> 2));
  */
  //Byte 0: lower 6 bits are in c0
  //        upper two bits are lower two bits of c1
  //Byte 1: lower 4 bits are upper four bits of c1
  //        upper 4 bits are lower 4 bits of c2
  //Byte 2: lower 2 bits are upper 2 bits of c2
  //        upper 6 bits are lower 6 bits of c3
  
  //Convert
  size_t len = strlen(b64str);
  size_t maxBytes = (size_t) (ceil((double)len/4)*3);
  size_t truesize = 0;
  byte* bytes = (byte*) malloc(sizeof(byte)*maxBytes);
  size_t b64i = 0;
  for (; b64i < len; b64i += 4) {
    if (b64i+3 < len) {
      int c0 = b64CharToInt(b64str[b64i+0]),
          c1 = b64CharToInt(b64str[b64i+1]),
          c2 = b64CharToInt(b64str[b64i+2]),
          c3 = b64CharToInt(b64str[b64i+3]);
      byte b0 = ((c0 & 0b00111111) << 2) + ((c1 & 0b00110000) >> 4),
           b1 = ((c1 & 0b00001111) << 4) + ((c2 & 0b00111100) >> 2),
           b2 = ((c2 & 0b00000011) << 6) + ((c3 & 0b00111111) >> 0);
      bytes[truesize++] = b0;
      bytes[truesize++] = b1;
      bytes[truesize++] = b2;
    }
  }
  
  //Handle final few characters (calcs copy/pasted from above, keep in sync)
  b64i -= 4;
  size_t charactersLeft = len-b64i;
  if (charactersLeft == 1) {
    printf("ERROR - Invalid base64 data");
    assert(0);
  }
  if (charactersLeft == 2) {
    int c0 = b64CharToInt(b64str[b64i+0]),
        c1 = b64CharToInt(b64str[b64i+1]);
    byte b0 = ((c0 & 0b00111111) << 2) + ((c1 & 0b00110000) >> 4);
    bytes[truesize++] = b0;
  }
  if (charactersLeft == 3) {
    int c0 = b64CharToInt(b64str[b64i+0]),
        c1 = b64CharToInt(b64str[b64i+1]),
        c2 = b64CharToInt(b64str[b64i+2]);
    byte b0 = ((c0 & 0b00111111) << 2) + ((c1 & 0b00110000) >> 4),
         b1 = ((c1 & 0b00001111) << 4) + ((c2 & 0b00111100) >> 2);
    bytes[truesize++] = b0;
    bytes[truesize++] = b1;
  }
  *n = truesize;
  
  return bytes;
}

byte* singleByteXor (const byte* input, byte key, size_t n) {
  byte* plaintext = (byte*) memcpy(malloc(n*sizeof(byte)), input, n);
  
  for (size_t i = 0; i < n; i++)
    plaintext[i] ^= key;
  
  return plaintext;
}

byte* repeatingKeyXor (const byte* input, size_t n, const byte* key, size_t keyn) {
  byte* plaintext = (byte*) memcpy(malloc(n*sizeof(byte)), input, n);
  
  for (size_t pi = 0, ki = 0; pi < n; pi++) {
    plaintext[pi] ^= key[ki];
    if(++ki >= keyn)
      ki=0;
  }
  
  return plaintext;
}

double differenceFromEnglish (const byte* text, size_t len) {
  size_t* textFreqs = getByteFrequencies(text, len);
  int score = compareByteFrequencies(textFreqs, byteFrequencies_englishText);
  free(textFreqs);
  return score;
}

size_t* getByteFrequencies (const byte* bytes, size_t len) {
  size_t* bins = (size_t*) calloc(256, sizeof(size_t));
  for (size_t i = 0; i < len; i++)
    bins[bytes[i]]++;
  return bins;
}

double compareByteFrequencies (const size_t* a, const size_t* b) {
  //Normalize frequencies to between 0 and 1000 and sum sqrt of absolute
  //  differences between frequences of same bytes. Sqrt because high similarity
  //  for some bytes is more important than high difference between others.
  size_t total_in_a = 0;
  size_t total_in_b = 0;
  for (size_t i = 0; i < 256; i++) {
    total_in_a += a[i];
    total_in_b += b[i];
  }
  
  double difference = 0;
  for (size_t i = 0; i < 256; i++) {
    difference += sqrt(abs((((double)a[i])*1000)/total_in_a -
                           (((double)b[i])*1000)/total_in_b ));
  }
  
  return difference;
}

size_t* getFrequenciesOfFile(const char* filename, bool printFreqs) {
  using namespace std;
  //Read file
  ifstream file (filename, ios::binary|ios::ate);
  size_t end = file.tellg();
  byte* bytes = (byte*) malloc(end*sizeof(byte));
  file.seekg(0, file.beg);
  file.read((char*)bytes, end);
  file.close();
  
  //Get frequencies
  size_t* freqs = getByteFrequencies(bytes, end);
  
  //Optionally output frequencies
  if (printFreqs) {
    printf("extern size_t byteFrequencies_myText[];\n");
    printf("size_t byteFrequencies_myText[] = {");
    for (size_t i = 0; i < 256; i++)
      printf("%zu,", freqs[i]);
    printf("\b};\n");
  }
  
  //Return
  free(bytes);
  return freqs;
}

double singleByteXorScore (const byte* ciphertext, byte key, size_t len, const size_t* byteFreqs) {
  byte* plaintext = singleByteXor(ciphertext, key, len);
  size_t* textFreqs = getByteFrequencies(plaintext, len);
  int score = compareByteFrequencies(textFreqs, byteFreqs);
  free(textFreqs);
  free(plaintext);
  return score;
}

byte* breakSingleByteXor (const byte* ciphertext, size_t len, bool log, size_t indentLevel, const size_t* byteFreqs, double threshold) {
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
    //PAD printf("Key: 0x%02x\n", bestKey);
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

void withTokens (const char* filename, void (*f)(const char* token)) {
  std::ifstream file(filename);
  std::string token;
  while (file >> token)
    (*f)(token.c_str());
}

size_t editDistanceByte (byte a, byte b) {
  //Xor and then count the ones
  size_t count = 0;
  for (int bit = 0; bit < 8; bit++)
    if((a^b)&(1<<bit))
      ++count;
  return count;
}

size_t editDistance (const byte* a, const byte* b, size_t n) {
  size_t count = 0;
  for (size_t i = 0; i < n; i++)
    count += editDistanceByte(a[i], b[i]);
  return count;
}

size_t editDistanceStr (const char* a, const char* b) {
  assert(strlen(a)==strlen(b));
  return editDistance((byte*)a, (byte*)b, strlen(a));
}

void printSpaces (size_t n) {
  for (size_t i = 0; i < n; i++)
    printf(" ");
}

char replaceUnprintableWithQuestion (char c) {
  return (c < ' ' || c > '~') ? '?' : c;
}

std::string _temp_buildingBase64Str;

void ignoreNonBase64AndAppend (const char* token) {
  size_t len = strlen(token);
  for (size_t i = 0; i < len; i++)
    if (isValidB64Char(token[i]))
      _temp_buildingBase64Str += token[i];
}

byte* getBase64DataFromFileByIgnoringAllElse(const char* filename, size_t* length) {
  _temp_buildingBase64Str = "";
  withTokens(filename, &ignoreNonBase64AndAppend);
  byte* data = b64StrToByteStr(_temp_buildingBase64Str.c_str(), length);
  _temp_buildingBase64Str = "";
  return data;
}

bool isValidB64Char (char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'A' && c <= 'Z') ||
         (c >= 'a' && c <= 'z') ||
         (c == '+' || c == '/');
}
