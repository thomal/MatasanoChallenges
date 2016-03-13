#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
typedef uint8_t byte; /* unsigned char may be >= 8 bits */

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
byte* hexStrToByteStr (char* hexstr, size_t* n) {
  *n = ceil(strlen(hexstr)/2.0);
  byte* bytestr = (byte*) calloc(*n, sizeof(byte));
  
  for (size_t ib = 0, ih = 0; ib < *n; ib++, ih+=2) {
    if (ib == 0 && ((strlen(hexstr) % 2) != 0))
      bytestr[ib] = hexDigitToInt(hexstr[ih--]);
    else
      bytestr[ib] = (hexDigitToInt(hexstr[ih])<<4) + hexDigitToInt(hexstr[ih+1]);
  }
  
  printf("ASCII (hexstr): ");
  for (size_t i = 0; i < *n; i++)
    printf("%c", bytestr[i]);
  printf("\n");
  
  return bytestr;
}

int main (int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: %s <hexstring> <hexstring>\n", argv[0]);
    exit(0);
  }
  
  size_t str1n;
  byte* str1 = hexStrToByteStr(argv[1], &str1n);
  size_t str2n;
  byte* str2 = hexStrToByteStr(argv[2], &str2n);
  
  if (str1n != str2n) {
    printf("ERROR: Cannot XOR differently sized inputs.\n");
    exit(0);
  }
  
  for (size_t i = 0; i < str1n; i++)
    str1[i] ^= str2[i];
  
  printf("Result: 0x");
  for (size_t i = 0; i < str1n; i++)
    printf("%x", str1[i]);
  printf("\nResult ASCII: ");
  for (size_t i = 0; i < str1n; i++)
    printf("%c", str1[i]);
  printf("\n");
  
  free(str1);
  free(str2);
  return 0;
}
