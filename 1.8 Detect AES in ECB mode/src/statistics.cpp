#include "cryptopals_common.h"

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
