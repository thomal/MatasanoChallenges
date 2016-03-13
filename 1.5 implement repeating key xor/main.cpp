#include "cryptopals_common.h"

void optionParsing(int argc, char** argv);

// -k hex/-k raw  determines how key is read
// -p hex/-p raw  determines how plaintext is read
// -o hex/-o raw  determines how ciphertext is output
// -x "password"
// -y "The message/plaintext goes here."
bool keyRaw    = true,
     msgRaw    = true,
     outputRaw = false;
byte *key, *plaintext;
size_t keyn, plaintextn;

int main (int argc, char** argv) {
  optionParsing(argc, argv);
  
  //Encrypt
  for (size_t i = 0; i < plaintextn; i++)
    plaintext[i] ^= key[i%keyn];
  
  //Output
  if (outputRaw) {
    for (size_t i = 0; i < plaintextn; i++)
      printf("%c", plaintext[i]);
    printf("\n");
  } else {
    for (size_t i = 0; i < plaintextn; i++)
      printf("%02x", plaintext[i]);
    printf("\n");
  }
  
  //Cleanup
  if (!msgRaw)
    free(plaintext);
  if (!keyRaw)
    free(key);
  return 0;
}

void optionParsing(int argc, char** argv) {
  int oc; //Option character
  char *key_, *plaintext_;
  
  if (argc == 1) {
    printf("Read main.cpp\n");
    exit(0);
  }
  
  while ((oc = getopt(argc, argv, "k:p:o:x:y:")) != -1) {
    switch (oc) {
    case 'k':
      keyRaw = strcmp(optarg,"hex");
      break;
    case 'p':
      msgRaw = strcmp(optarg,"hex");
      break;
    case 'o':
      outputRaw = strcmp(optarg,"hex");
      break;
    case 'x':
      key_ = optarg;
      break;
    case 'y':
      plaintext_ = optarg;
      break;
    default:
      printf("Unknown argument: %c\n", oc);
      printf("Read main.cpp\n");
      exit(0);
      break;
    }
  }
  
  if (keyRaw) {
    assert(sizeof(byte)==sizeof(char));
    key = (byte*)key_;
    keyn = strlen(key_);
  } else {
    key = hexStrToByteStr(key_, &keyn);
  }
  
  if (msgRaw) {
    assert(sizeof(byte)==sizeof(char));
    plaintext = (byte*)plaintext_;
    plaintextn = strlen(plaintext_);
  } else {
    plaintext = hexStrToByteStr(plaintext_, &plaintextn);
  }
}
