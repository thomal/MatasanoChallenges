#include "parse_options.h"
#define CASE_X_SET_Y_IF_OPTARG_ISNT_Z(X,Y,Z) \
  case (X): \
    (Y) = strcmp(optarg, (Z)); \
    break;

#define CASE_X_SET_Y_TO_OPTARG(X,Y) \
  case (X): \
    (Y) = optarg; \
    break;
                

bool keyRaw    = true,
     inputRaw  = true,
     outputRaw = false;
byte *key, *input;
size_t keyn, inputn;

void optionParsing(int argc, char** argv) {
  int oc; //Option character
  char *key_, *input_;
  
  if (argc == 1) {
    printf("Read main.cpp\n");
    exit(0);
  }
  
  while ((oc = getopt(argc, argv, "k:i:o:x:y:")) != -1) {
    switch (oc) {
    CASE_X_SET_Y_IF_OPTARG_ISNT_Z('k', keyRaw,    "hex")
    CASE_X_SET_Y_IF_OPTARG_ISNT_Z('i', inputRaw,  "hex")
    CASE_X_SET_Y_IF_OPTARG_ISNT_Z('o', outputRaw, "hex")
    CASE_X_SET_Y_TO_OPTARG('x', key_)
    CASE_X_SET_Y_TO_OPTARG('y', input_)
    default:
      printf("Unknown argument: %c\n", oc);
      printf("Read parse_options.h\n");
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
  
  if (inputRaw) {
    assert(sizeof(byte)==sizeof(char));
    input = (byte*)input_;
    inputn = strlen(input_);
  } else {
    input = hexStrToByteStr(input_, &inputn);
  }
}

void cleanupArgs() {
  if (!inputRaw)
    free(input);
  if (!keyRaw)
    free(key);
}
