#include "cryptopals_common.h"

size_t byteFrequencies_englishText[] =  {0,0,0,0,0,0,0,0,0,0,10661,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95154,31,890,1,60,1,31,1418,37,37,36,0,4977,1028,14508,12,294,620,322,191,165,181,142,141,143,225,21,84,0,0,0,151,1,1138,587,507,328,450,393,304,649,3262,366,57,278,482,375,542,536,6,377,837,1342,115,41,844,5,133,1,1,0,1,0,10,0,32434,6138,8638,19031,46978,7727,8690,23171,21705,596,4453,16058,9763,26780,32707,5925,226,21587,20199,36955,11645,2853,11863,567,7278,181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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

byte* singleByteXor (byte* ciphertext, byte key, size_t len) {
  byte* plaintext = (byte*) memcpy(malloc(len*sizeof(byte)), ciphertext, len);
  
  for (size_t i = 0; i < len; i++)
    plaintext[i] ^= key;
  
  return plaintext;
}

double differenceFromEnglish (byte* text, size_t len) {
  size_t* textFreqs = getByteFrequencies(text, len);
  int score = compareByteFrequencies(textFreqs, byteFrequencies_englishText);
  free(textFreqs);
  return score;
}

size_t* getByteFrequencies (byte* bytes, size_t len) {
  size_t* bins = (size_t*) calloc(256, sizeof(size_t));
  for (size_t i = 0; i < len; i++)
    bins[bytes[i]]++;
  return bins;
}

double compareByteFrequencies (size_t* a, size_t* b) {
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
  assert(sizeof(uint8_t) == sizeof(char));
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

double singleByteXorScore (byte* ciphertext, byte key, size_t len, size_t* byteFreqs) {
  byte* plaintext = singleByteXor(ciphertext, key, len);
  size_t* textFreqs = getByteFrequencies(plaintext, len);
  int score = compareByteFrequencies(textFreqs, byteFreqs);
  free(textFreqs);
  free(plaintext);
  return score;
}

byte* breakSingleByteXOR (byte* ciphertext, size_t len, size_t* byteFreqs, double threshold) {
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
  printf("Score: %f\n", minScore);
  printf("Key: %x\n", bestKey);
  printf("Key ASCII: %c\n", bestKey);
  printf("Plaintext: ");
  for (size_t i = 0; i < len; i++)
    printf("%x", plaintext[i]);
  printf("\nPlaintext ASCII: ");
  for (size_t i = 0; i < len; i++)
    printf("%c", plaintext[i]);
  printf("\n");
  
  return plaintext;
}

void withFile (const char* filename, void (*f)(const char* line)) {
  std::ifstream file(filename);
  std::string line;
  while (file >> line)
    (*f)(line.c_str());
}
