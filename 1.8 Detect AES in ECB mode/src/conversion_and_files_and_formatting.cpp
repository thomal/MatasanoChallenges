#include "cryptopals_common.h"

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

void withTokens (const char* filename, void (*f)(const char* token)) {
  std::ifstream file(filename);
  std::string token;
  while (file >> token)
    (*f)(token.c_str());
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
