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
byte *_input  = nullptr,
     *_input2 = nullptr;
size_t input2n;
char* mode = nullptr;
bool f_log = false;
IOFormat outputF = UNDEFINED;
byte* (*withInputData) (const byte* input, size_t in, const byte* input2, size_t i2n, const char* mode, bool log, size_t* outn) = nullptr;

void optionParsing(int argc, char** argv, byte* (*f) (const byte* input, size_t in, const byte* input2, size_t i2n, const char* mode, bool log, size_t* outn)) {
  int oc; //Option character
  withInputData = f;
  
  if (argc == 1) {
    printf("Read INTERFACE file.\n");
    exit(0);
  }
  
  //Store input
  IOFormat inputF           = UNDEFINED,
           input2F          = UNDEFINED;
  char *input  = nullptr,
       *input2 = nullptr;
  size_t inputn;
  
  //Parse args
  while ((oc = getopt(argc, argv, "i:I:o:y:x:m:l")) != -1) {
    switch (oc) {
    FORMAT_ARGUMENT('i', inputF)
    FORMAT_ARGUMENT('I', input2F)
    FORMAT_ARGUMENT('o', outputF)
    CASE_X_SET_Y_TO_OPTARG('y', input)
    CASE_X_SET_Y_TO_OPTARG('x', input2)
    CASE_X_SET_Y_TO_OPTARG('m', mode)
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
  if (input2F != HEX && input2F != RAW) {
    printf("ERROR - must specify input format with -I [raw|hex]\n");
    exit(0);
  }
  if (outputF != HEX && outputF != RAW) {
    printf("ERROR - must specify output format with -o [raw|hex]\n");
    exit(0);
  }
  if (input == nullptr) {
    printf("ERROR - must specify input data with -y [<hex string>|<raw bytes>|<filename>]\n");
    exit(0);
  }
  if (input2 == nullptr) {
    printf("ERROR - must specify input data with -x [<hex string>|<raw bytes>]\n");
    exit(0);
  }
  if (mode == nullptr || (strcmp(mode, "add") && strcmp(mode, "remove"))) {
    printf("ERROR - must specify mode with -m [add|remove]\n");
    exit(0);
  }
  
  //Duplicate arguments on the heap
  //Duplicate mode
  mode = strdup(mode);
  //Duplicate input2
  if (input2F == RAW) {
    _input2 = (byte*)strdup(input2);
    input2n = strlen(input2);
  } else if (input2F == HEX) {
    _input2 = hexStrToByteStr(input2, &input2n);
  }
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
  
  if (inputF != HEXFILE_TOKENS) {
    size_t len;
    byte* output = (*withInputData)(_input, inputn, _input2, input2n, mode, f_log, &len);
    convertOutputFree(output, len, outputF);
  }
}

void tokensFromHexFile (const char* hexData) {
  static bool firstRun = true;
  if (!firstRun) {
    printf("\n");
  } else {
    firstRun = false;
  }
  size_t inputn;
  byte* input = hexStrToByteStr(hexData, &inputn);
  size_t len;
  byte* output = (*withInputData)(input, inputn, _input2, input2n, mode, f_log, &len);
  convertOutputFree(output, len, outputF);
  free(input);
}

void convertOutputFree (byte* data, size_t n, IOFormat format) {
  if (f_log)
    printf("Output: ");
  if (format == RAW) {
    for (size_t i = 0; i < n; i++)
      printf("%c", data[i]);
    if (f_log)
      printf("\n");
  } else if (format == HEX) {
    for (size_t i = 0; i < n; i++)
      printf("%02x", data[i]);
    if (f_log)
      printf("\n");
  } else {
    printf("ERROR - Unsupported output format. Must be either \"hex\" or \"raw\"\n");
    assert(0);
  }
  
  free(data);
}

void cleanupArgs() {
  if (_input != nullptr)
    free(_input);
  if (_input2 != nullptr)
    free(_input2);
  if (mode != nullptr)
    free(mode);
}
