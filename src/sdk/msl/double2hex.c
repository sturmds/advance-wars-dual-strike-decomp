#include "types.h"
#include "sdk/msl.h"
#include "sdk/msl_string.h"

/*
 * Built with mwccarm 2.0 build 56 (config/AWRE/compilers.txt, issue 0061):
 * 2.0-72 sets up an r11 frame to pass expFormat by value, and the original
 * has none.
 */

#define DOUBLE_EXPONENT_MASK 0x7FF
#define DOUBLE_EXPONENT_BIAS 1023

extern char data_0216df30[]; /* "0x0p0" */
#pragma explicit_zero_data on
/* declared `const wchar_t data_0216def8[]` in __pformatter.c: type const wchar_t is not one the tool can write */
u32 data_0216def8 = {
#include "data/main/data_0216def8.inc.c"
};
/* "" */
char data_0216defc[4] = {
#include "data/main/data_0216defc.inc.c"
};
/* "-INF" */
char data_0216df00[5] = {
#include "data/main/data_0216df00.inc.c"
};
/* "-inf" */
char data_0216df08[5] = {
#include "data/main/data_0216df08.inc.c"
};
#pragma explicit_zero_data reset
extern char data_0216df10[]; /* "INF" */
extern char data_0216df14[]; /* "inf" */
extern char data_0216df18[]; /* "-NAN" */
extern char data_0216df20[]; /* "-nan" */
extern char data_0216df28[]; /* "NAN" */
extern char data_0216df2c[]; /* "nan" */

char* double2hex(double value, char* bufferEnd, PrintFormat format) {
  char* p;
  u8* bytes;
  u8 digit;
  u8 swap;
  u32 high;
  long exponent;
  int hexDigits;
  int bit;
  int i;
  u8* swapBytes;
  DecimalFormat form;
  PrintFormat expFormat;
  Decimal dec;

  if (format.precision > PRINT_CONVERSION_MAX) {
    return NULL;
  }

  form.style = 0;
  form.digits = DECIMAL_SIG_DIGITS;
  __num2dec(&form, value, &dec);

  if (fabs(value) == 0.0) {
    p = bufferEnd - 6;
    strcpy(p, data_0216df30);
    return p;
  }

  if (dec.digits[0] == 'I') {
    if (dec.sign) {
      p = bufferEnd - 5;
      if (format.conversionChar == 'A') {
        strcpy(p, data_0216df00);
      } else {
        strcpy(p, data_0216df08);
      }
    } else {
      p = bufferEnd - 4;
      if (format.conversionChar == 'A') {
        strcpy(p, data_0216df10);
      } else {
        strcpy(p, data_0216df14);
      }
    }
    return p;
  }
  if (dec.digits[0] == 'N') {
    if (dec.sign) {
      p = bufferEnd - 5;
      if (format.conversionChar == 'A') {
        strcpy(p, data_0216df18);
      } else {
        strcpy(p, data_0216df20);
      }
    } else {
      p = bufferEnd - 4;
      if (format.conversionChar == 'A') {
        strcpy(p, data_0216df28);
      } else {
        strcpy(p, data_0216df2c);
      }
    }
    return p;
  }

  expFormat.justification = PRINT_JUSTIFY_RIGHT;
  expFormat.signOptions = PRINT_SIGN_ALWAYS;
  expFormat.precisionSpecified = 0;
  expFormat.alternateForm = 0;
  expFormat.argumentOptions = PRINT_ARG_NORMAL;
  expFormat.fieldWidth = 0;
  expFormat.precision = 1;
  expFormat.conversionChar = 'd';

  /* Byte-reverse the double so the sign and exponent come first. Needed to
   * match: the swap and the digit loop use separate pointers, declared in
   * this order, and the exponent reads value directly (the original sets up
   * &value twice) in two steps. */
  swapBytes = (u8*)&value;
  for (i = 0; i < 4; i++) {
    swap = swapBytes[i];
    swapBytes[i] = swapBytes[7 - i];
    swapBytes[7 - i] = swap;
  }

  high = (((u8*)&value)[0] << 25) | (((u8*)&value)[1] << 17);
  high = (high >> 21) & DOUBLE_EXPONENT_MASK;
  exponent = high - DOUBLE_EXPONENT_BIAS;
  p = long2str(exponent, bufferEnd, expFormat);
  if (format.conversionChar == 'a') {
    *--p = 'p';
  } else {
    *--p = 'P';
  }

  bytes = (u8*)&value;
  /* Mantissa bits follow the sign bit and 11 exponent bits; print 4 per digit, last digit first. */
  bit = format.precision * 4 + 11;
  for (hexDigits = format.precision; hexDigits >= 1; hexDigits--) {
    if (bit < 64) {
      digit = bytes[bit >> 3] >> (7 - (bit & 7));
      if ((bit & ~7) != ((bit - 4) & ~7)) {
        digit |= (bytes[(bit >> 3) - 1] << 8) >> (7 - (bit & 7));
      }
      digit &= 0xF;
      if (digit < 10) {
        digit += '0';
      } else if (format.conversionChar == 'a') {
        digit += 'a' - 10;
      } else {
        digit += 'A' - 10;
      }
    } else {
      digit = '0';
    }
    *--p = digit;
    bit -= 4;
  }

  if (format.precision != 0 || format.alternateForm) {
    *--p = '.';
  }
  *--p = '1';
  if (format.conversionChar == 'a') {
    *--p = 'x';
  } else {
    *--p = 'X';
  }
  *--p = '0';

  if (dec.sign) {
    *--p = '-';
  } else if (format.signOptions == PRINT_SIGN_ALWAYS) {
    *--p = '+';
  } else if (format.signOptions == PRINT_SIGN_SPACE) {
    *--p = ' ';
  }
  return p;
}
