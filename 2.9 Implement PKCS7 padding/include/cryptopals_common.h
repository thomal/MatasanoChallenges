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
#include <unistd.h>
#include <inttypes.h>
typedef uint8_t byte; /* unsigned char may be >= 8 bits */

// Defaults ////////////////////////////////////////////////////////////////////
#define DEFAULT_FREQUENCY_SIMILARITY_THRESHOLD 200
#define DEFAULT_BYTE_FREQUENCIES byteFrequencies_englishText
////////////////////////////////////////////////////////////////////////////////

// MACROS //////////////////////////////////////////////////////////////////////
#define PAD printSpaces(indent*4);
////////////////////////////////////////////////////////////////////////////////

// Globals /////////////////////////////////////////////////////////////////////
//Text frequencies in normal English text. It's important to note that normal
//  English text isn't all lower or upper case and includes a great many
//  newlines.
extern const size_t byteFrequencies_englishText[];
// Indent for logging messages
extern size_t indent;
// For base64
extern char base64arr[];
////////////////////////////////////////////////////////////////////////////////

// Encoding ////////////////////////////////////////////////////////////////////
//Allocates new buffer to hold byte*, puts size in *n
byte* hexStrToByteStr (const char* hexstr, size_t* n);
//Determines whether c is a valid character in base64
bool isValidB64Char (char c);
//Allocates new buffer to hold byte*, puts size in *n
byte* b64StrToByteStr (const char* b64str, size_t* n);
//Converts a base64 character c into the int it represents. Returns -1 on fail.
// '=' is not a base64 character, it is unrequired padding.
int b64CharToInt (char c);
//PKCS7 Padding
//Return value must be free()'d.
byte* PKCS7_add(const byte* input, size_t in, uint8_t blocksize, size_t* outn);
byte* PKCS7_remove(const byte* input, size_t in, uint8_t blocksize, size_t* outn);
////////////////////////////////////////////////////////////////////////////////

// Encryption & Decryption /////////////////////////////////////////////////////
//Allocates new buffer to hold byte*, n is length of input in bytes. The size of
//  the output is equal to n, and the same function is used for encryption and
//  decryption.
byte* singleByteXor (const byte* input, byte key, size_t n);
//Allocates new buffer to hold byte*, n is length of input in bytes, keyn is the
// length of key in bytes. The size of the output is equal to n, and the same
// function is used for encryption and decryption.
byte* repeatingKeyXor (const byte* input, size_t n, const byte* key, size_t keyn);
//Allocates a new buffer to hold byte*, in is length of input in bytes, kn is
//  length of key in bytes. kn is the size of the key in bytes. Keysize must be
//  128, 192, or 256 bits. in must be a multiple of 16, and padding must be
//  taken care of by the user.
byte* aes_ecb_encrypt(const byte* input, size_t in, const byte* key, size_t kn, bool log);
byte* aes_ecb_decrypt(const byte* input, size_t in, const byte* key, size_t kn, bool log);
//See the documentation for aes_ecb_encrypt/decrypt for an explanation of args.
byte* aes_encrypt_block (const byte* block_in, const byte* key, size_t kn, bool log);
byte* aes_decrypt_block (const byte* block_in, const byte* key, size_t kn, bool log);
////////////////////////////////////////////////////////////////////////////////

// Breaks //////////////////////////////////////////////////////////////////////
// Takes a ciphertext from singleByteXor() and breaks it if there is a known
//   frequency distribution for bytes in the plaintext. The threshold value
//   determines the maximum score at which a key will still be considered
//   correct. If the lowest scoring key (best) is above the threshold then the
//   attempt is consiered to have failed and the function returns nullptr.
//   Set byteFreqs to nullptr to use the default.
byte* breakSingleByteXor (const byte* ciphertext, size_t len, bool log = false, const size_t* byteFreqs = DEFAULT_BYTE_FREQUENCIES, double threshold = DEFAULT_FREQUENCY_SIMILARITY_THRESHOLD);
// Takes a ciphertext from repeatingKeyXor() and breaks it if there is a known
//   frequency distribution for bytes in the plaintext. The threshold value
//   determines the maximum score at which a key will still be considered
//   correct when calling breakSingleByteXor.
// This function returns nullptr on failure.
// Set byteFreqs to nullptr to use the default.
// Set minKeysize or maxKeysize to 0 to use the respective default which is set
//   in the function itself.
byte* breakRepeatingKeyXor (const byte* ciphertext, size_t len, bool log = false, const size_t* byteFreqs = DEFAULT_BYTE_FREQUENCIES, size_t minKeysize = 0, size_t maxKeysize = 0, double threshold = DEFAULT_FREQUENCY_SIMILARITY_THRESHOLD);
////////////////////////////////////////////////////////////////////////////////

// Heuristics //////////////////////////////////////////////////////////////////
// Returns an array of 256 size_ts that must be free()'d. Offset n is the number
//   of occurances of n in bytes.
size_t* getByteFrequencies (const byte* bytes, size_t len);

// Return result is comparable to return results from compareByteFrequencies.
//   Score is based on letter frequency similarity. Lower is more similar.
double differenceFromEnglish (const byte* text, size_t len);

// Comparablability of return result is reflexive and transitive. Return result
//   can be compared with other return results from this function, and possibly
//   those of other functions if they say so. Thus two functions whose return
//   results are comparable with the return result of this function also have
//   comparable return results.
// a and b are byte frequencies as returned by getByteFrequencies.
// Lower scores indicate similarity.
double compareByteFrequencies (const size_t* a, const size_t* b);

// Load a file, and count the byte frequencies within. Optionally print the
//   frequences to stdout in a form that can be copy/pasted into a C program.
// The return value must be free()'d.
size_t* getFrequenciesOfFile(const char* filename, bool printFreqs = false);
// Find edit distance (in bits) between a and b.
size_t editDistanceByte (byte a, byte b);
// Find edit distance (in bits) between two arrays of bytes of equal size.
//   Each bytestring must be n bytes long.
size_t editDistance (const byte* a, const byte* b, size_t n);
// This just casts a and b to byte*s and passes them to editDistance. It calls
//   strlen everytime so DON'T USE THIS IN A TIGHT LOOP!
// The following MUST hold: strlen(a)==strlen(b), beware of null bytes.
size_t editDistanceStr (const char* a, const char* b);
// Returns true if AES in ECB mode is detected, false otherwise.
bool aes_ecb_detect (const byte* input, size_t in, bool log);
// Counts the number of duplicate blocks that occur when input is divided into
//   equal size blocks of block_size.
// e.g. when block_size = 3 and input = 012013012033012 then the division is:
//   012 013 012 033 012 which has two duplicate blocks. size=1,input=5955 has 2
size_t count_duplicate_blocks (const byte* input, size_t in, bool log, size_t block_size);
////////////////////////////////////////////////////////////////////////////////

// Utility /////////////////////////////////////////////////////////////////////
// Call the given function (void foo (const char* line)) with each token of the
//   specified file in turn. NB: The delimiter is any whitespace.
void withTokens (const char* filename, void (*f)(const char* token));
//Prints n spaces and no newline.
void printSpaces (size_t n);
//Replace non-printable ASCII characters with question marks
char replaceUnprintableWithQuestion (char c);
//Ignore everything in the file except valid base64 characters, and treat these
//  as one long datum. Ignores newlines, &c.
byte* getBase64DataFromFileByIgnoringAllElse(const char* filename, size_t* length);
////////////////////////////////////////////////////////////////////////////////
#endif
