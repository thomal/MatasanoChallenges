#include "parse_options.h"
#define SET_X_TO_Y_IF_OPTARG_IS_S(X,Y,S) \
    if (strcmp(optarg, (S)) == 0) \
      (X) = (Y);

#define ASSIGN_ENUM_FROM_FORMAT_STRING_IN_OPTARG(X) \
  SET_X_TO_Y_IF_OPTARG_IS_S((X), RAW,            "raw") \
  SET_X_TO_Y_IF_OPTARG_IS_S((X), HEXFILE_TOKENS, "hexfile_tokens") \
  SET_X_TO_Y_IF_OPTARG_IS_S((X), HEX,            "hex") \
  SET_X_TO_Y_IF_OPTARG_IS_S((X), B64FILE,        "b64file")

#define FORMAT_ARGUMENT(X,Y) \
  case (X): \
      ASSIGN_ENUM_FROM_FORMAT_STRING_IN_OPTARG((Y)) \
      break;

#define CASE_X_SET_Y_TO_OPTARG(X,Y) \
  case (X): \
    (Y) = optarg; \
    break;
                
IOFormat inputF  = UNDEFINED,
         outputF = UNDEFINED;
byte *input;
size_t inputn;
IOFormat inputF_original = UNDEFINED;

void optionParsing(int argc, char** argv) {
  int oc; //Option character
  char *input_ = nullptr;
  
  if (argc == 1) {
    printf("Read parse_options.h/.cpp\n");
    exit(0);
  }
  
  //Parse args
  while ((oc = getopt(argc, argv, "i:o:y:")) != -1) {
    switch (oc) {
    FORMAT_ARGUMENT('i', inputF)
    FORMAT_ARGUMENT('o', outputF)
    CASE_X_SET_Y_TO_OPTARG('y', input_)
    default:
      printf("Unknown argument: %c\n", oc);
      printf("Read parse_options.h\n");
      exit(0);
      break;
    }
  }
  
  //Deal with unprovided options and invalid combinations
  if (inputF != HEX && inputF != RAW && inputF != HEXFILE_TOKENS && inputF != B64FILE) {
    printf("ERROR - must specify input format with -i [raw|hex|hexfile_tokens|b64file]\n");
    exit(0);
  }
  if (outputF != HEX && outputF != RAW) {
    printf("ERROR - must specify output format with -o [raw|hex]\n");
    exit(0);
  }
  if (input_ == nullptr) {
    printf("ERROR - must specify input data with -y [<hex string>|<raw bytes>|<filename>]\n");
    exit(0);
  }
  
  //Set input and inputn
  if (inputF == RAW) {
    input = (byte*)input_;
    inputn = strlen(input_);
  } else if (inputF == HEX) {
    input = hexStrToByteStr(input_, &inputn);
    inputF = RAW;
    inputF_original = HEX;
  } else if (inputF == B64FILE) {
    input = getBase64DataFromFileByIgnoringAllElse(input_, &inputn);
    inputF = RAW;
    inputF_original = B64FILE;
  }
}

void cleanupArgs() {
  if (inputF_original == HEX || inputF_original == B64FILE)
    free(input);
}
