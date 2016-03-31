#include "parse_options.h"
#include "cryptopals_common.h"
typedef enum {UNDEFINED, RAW, HEX, HEXFILE_TOKENS, B64FILE} IOFormat;
void tokensFromHexFile (const char* hexData);
void convertOutputFree (byte* data, size_t n, IOFormat format);

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

#define FLAG(X,Y) \
  case (X): \
    (Y) = true;\
    break;

//Required for tokensFromHexFile
byte *_input  = nullptr;
bool f_log = false;
void (*withInputData) (const byte* input, size_t in, bool log) = nullptr;

void optionParsing(int argc, char** argv, void (*f) (const byte* input, size_t in, bool log)) {
  int oc; //Option character
  withInputData = f;
  
  if (argc == 1) {
    printf("Read INTERFACE file.\n");
    exit(0);
  }
  
  //Store input
  IOFormat inputF = UNDEFINED;
  char *input  = nullptr;
  size_t inputn;
  
  //Parse args
  while ((oc = getopt(argc, argv, "i:y:l")) != -1) {
    switch (oc) {
    FORMAT_ARGUMENT('i', inputF)
    CASE_X_SET_Y_TO_OPTARG('y', input)
    FLAG('l', f_log)
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
  if (input == nullptr) {
    printf("ERROR - must specify input data with -y [<hex string>|<raw bytes>|<filename>]\n");
    exit(0);
  }
  
  //Duplicate arguments on the heap
  //Duplicate input
  if (inputF == RAW) {
    _input = (byte*)strdup(input);
    inputn = strlen(input);
  } else if (inputF == HEX) {
    _input = hexStrToByteStr(input, &inputn);
  } else if (inputF == B64FILE) {
    _input = getBase64DataFromFileByIgnoringAllElse(input, &inputn);
  } else if (inputF == HEXFILE_TOKENS) {
    _input = (byte*)strdup(input);
    withTokens((const char*)_input, &tokensFromHexFile);
  }
  
  if (inputF != HEXFILE_TOKENS)
    (*withInputData)(_input, inputn, f_log);
}

void tokensFromHexFile (const char* hexData) {
  size_t inputn;
  byte* input = hexStrToByteStr(hexData, &inputn);
  (*withInputData)(input, inputn, f_log);
  free(input);
}

void cleanupArgs() {
  if (_input != nullptr)
    free(_input);
}
