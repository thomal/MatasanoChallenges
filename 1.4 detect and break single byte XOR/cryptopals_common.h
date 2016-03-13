#ifndef CRYPTOPALS_COMMON_H
#define CRYPTOPALS_COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <assert.h>
#include <string>
#define DEFAULT_FREQUENCY_SIMILARITY_THRESHOLD 200
typedef uint8_t byte; /* unsigned char may be >= 8 bits */

// Globals /////////////////////////////////////////////////////////////////////
extern size_t byteFrequencies_englishText[];
////////////////////////////////////////////////////////////////////////////////

// Encoding ////////////////////////////////////////////////////////////////////
//Allocates new buffer to hold byte*, puts size in *n
byte* hexStrToByteStr (const char* hexstr, size_t* n);
////////////////////////////////////////////////////////////////////////////////

// Encryption & Decryption /////////////////////////////////////////////////////
//Allocates new buffer to hold byte*, puts size in *outn, inn is size of
//  ciphertext.
byte* singleByteXor (byte* ciphertext, byte key, size_t len);
////////////////////////////////////////////////////////////////////////////////

// Breaks //////////////////////////////////////////////////////////////////////
// Takes a ciphertext which has been singleByteXor()'d and breaks it if there is
//   a known frequency distribution for bytes in the plaintext. The threshold
//   value determines the maximum score at which a key will still be considered
//   correct. If the lowest scoring key (best) is above the threshold then the
//   attempt is consiered to have failed and the function returns nullptr.
byte* breakSingleByteXOR (byte* ciphertext, size_t len, size_t* byteFreqs, double threshold = DEFAULT_FREQUENCY_SIMILARITY_THRESHOLD);
////////////////////////////////////////////////////////////////////////////////

// Heuristics///////////////////////////////////////////////////////////////////
// Returns an array of 256 size_ts that must be free()'d. Offset n is the number
//   of occurances of n in bytes.
size_t* getByteFrequencies (byte* bytes, size_t len);

// Return result is comparable to return results from compareByteFrequencies.
//   Score is based on letter frequency similarity. Lower is more similar.
double differenceFromEnglish (byte* text, size_t len);

// Comparablability of return result is reflexive and transitive. Return result
//   can be compared with other return results from this function, and possibly
//   those of other functions if they say so. Thus two functions whose return
//   results are comparable with the return result of this function also have
//   comparable return results.
// a and b are byte frequencies as returned by getByteFrequencies.
// Lower scores indicate similarity.
double compareByteFrequencies (size_t* a, size_t* b);

// Load a file, and count the byte frequencies within. Optionally print the
//   frequences to stdout in a form that can be copy/pasted into a C program.
// The return value must be free()'d.
size_t* getFrequenciesOfFile(const char* filename, bool printFreqs = false);
////////////////////////////////////////////////////////////////////////////////

// Utility /////////////////////////////////////////////////////////////////////
// Call the given function (void foo (const char* line)) with each line of the
//   specified file in turn.
void withFile (const char* filename, void (*f)(const char* line));
////////////////////////////////////////////////////////////////////////////////
#endif
