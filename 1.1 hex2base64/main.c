#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
typedef uint8_t byte; /* unsigned char may be >= 8 bits */

char base64arr[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                    'U', 'V', 'W', 'X', 'Y', 'Z',
                    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                    'u', 'v', 'w', 'x', 'y', 'z',
                    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                    '+', '/'};

#define A(X) (base64arr[(X)])

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

void reverse (char* str) {
  size_t n = strlen(str);
  for (size_t i = 0; i < floor(n/2); i++) {
    char c = str[i];
    str[i] = str[(n-1)-i];
    str[(n-1)-i] = c;
  }
}

int main (int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <hexstring>\n", argv[0]);
    exit(0);
  }
  
  //Convert hex->bytes
  char* hexstr = argv[1];
  size_t bytes = ceil(strlen(hexstr)/2.0);
  byte* bytestr = (byte*) calloc(bytes, sizeof(byte));
  
  for (size_t ib = 0, ih = 0; ib < bytes; ib++, ih+=2) {
    if (ib == 0 && ((strlen(hexstr) % 2) != 0))
      bytestr[ib] = hexDigitToInt(hexstr[ih--]);
    else
      bytestr[ib] = (hexDigitToInt(hexstr[ih])<<4) + hexDigitToInt(hexstr[ih+1]);
  }
  
  printf("ASCII: ");
  for (size_t i = 0; i < bytes; i++)
    printf("%c", bytestr[i]);
  printf("\n");
  
  //Convert bytes->base64
  size_t b64chars = ceil((bytes*8.0)/6.0);
  char* base64 = (char*) calloc(b64chars+1+2, sizeof(byte)); //+2 for padding
  
  //3 bytes == 24 bits == 4 base-64 chars
  size_t ib = 0, i64 = 0;
  while ((ib+2) < bytes) {
    //Collect relevent values locally
    i64 = (ib/3)*4; //Index of first base-64 character to set
    byte byte0 = bytestr[(bytes-1)-(ib+0)];
    byte byte1 = bytestr[(bytes-1)-(ib+1)];
    byte byte2 = bytestr[(bytes-1)-(ib+2)];
    //Get values of base-64 digits
    base64[i64+0] = A(  byte0 & 0b00111111);
    base64[i64+1] = A(((byte0 & 0b11000000) >> 6) | ((byte1 & 0b00001111) << 2));
    base64[i64+2] = A(((byte1 & 0b11110000) >> 4) | ((byte2 & 0b00000011) << 4));
    base64[i64+3] = A(((byte2 & 0b11111100) >> 2));
    ib+=3;
  }
  i64 = (ib/3)*4;
  
  //Handle final one or two bytes, and pad with '='
  if (ib < bytes) {
    //Only bytestr[ib]
    if (ib == bytes-1) {
      byte byte0 = bytestr[(bytes-1)-(ib+0)];
      base64[i64] = base64arr[byte0 & 0b00111111];
      //Reverse & Pad
      reverse(base64);
      base64[i64+1] = '=';
      base64[i64+2] = '=';
    } else if (ib == bytes-2) { //bytestr[ib], bytestr[ib+1]
      //Relevent values
      byte byte0 = bytestr[(bytes-1)-(ib+0)];
      byte byte1 = bytestr[(bytes-1)-(ib+1)];
      //Values of digits
      base64[i64] =   A(  byte0 & 0b00111111);
      base64[i64+1] = A(((byte0 & 0b11000000)>>6) | ((byte1 & 0b00001111)<<2));
      //Reverse & Pad
      reverse(base64);
      base64[i64+2] = '=';
    }
  } else {
    reverse(base64);
  }
  
  printf("Base64: %s\n", base64);
  
  free(bytestr);
  free(base64);
  return 0;
}
