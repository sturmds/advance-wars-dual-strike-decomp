/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#pragma opt_dead_code off
#include "types.h"
#include "sdk/isd.h"

int Isd_StringLength(const char* s);
void Isd_WriteSpaces(IsdPrintTarget* target, u32 count);
u32 Isd_DivideByRadix(u32 value, u32 radix);
int Isd_DoubleExponent(double x);

/* Software double helpers in src/sdk/isd/. */
u32 Isd_DoubleUnitsDigit(double x); /* the digit in front of the point */
double Isd_DoubleDivideByTen(
    double x,
    int keepFraction); /* x / 10; with keepFraction 0, the integer part of it */
double Isd_DoubleTimesTen(double x);      /* x * 10 */
double Isd_DoubleAdd(double a, double b); /* a + b */

extern char gIsdNullString[]; /* "(null)" */
extern char gIsdZeroString[]; /* "0" */
/* Parser tables: a class for each character, and the action for each (state, class); one byte used per halfword. */
extern u16 gIsdFormatActions[8][9];
extern u16 gIsdFormatCharClass[256];

/* Parser states. The action table gives the next state, so the same values name the actions. */
enum {
  STATE_TEXT,
  STATE_PERCENT,
  STATE_FLAGS,
  STATE_WIDTH,
  STATE_DOT,
  STATE_PRECISION,
  STATE_LENGTH,
  STATE_CONVERSION
};

enum {
  CONV_STRING,
  CONV_CHAR,
  CONV_DECIMAL,
  CONV_UNSIGNED,
  CONV_BINARY,
  CONV_OCTAL,
  CONV_HEX,
  CONV_HEX_UPPER,
  CONV_EXP,
  CONV_EXP_UPPER,
  CONV_FIXED,
  CONV_GENERAL,
  CONV_GENERAL_UPPER
};

#define FORMAT_LEFT 0x1
#define FORMAT_PLUS 0x2
#define FORMAT_ZERO_PAD 0x4
#define FORMAT_SPACE 0x8
#define FORMAT_ALTERNATE 0x10
#define FORMAT_SIGNED 0x1000
#define FORMAT_UNSIGNED 0x2000
#define FORMAT_NEGATIVE 0x4000

/* Exponent field of 1.0: a double is at least 1 when its exponent field is at least this. */
#define ISD_DOUBLE_EXPONENT_ONE 0x3ff

/*
 * Writes one byte with a halfword read and write, like Isd_ReadByte reads.
 * Needed to match: the copies of both arguments make the compiler work out
 * each argument once, before the alignment test (the copies in
 * isd_double_add.c and its neighbours have none). The void * parameter keeps
 * the compiler from sharing one alignment test between two stores to the same
 * address (the fraction digit loop).
 */
static inline void Isd_StoreByte(void* address, int value) {
  char* at = address;
  int byte = value;

  if ((u32)at & 1) {
    *(u16*)(at - 1) = (*(u16*)(at - 1) & 0x00ff) | ((byte & 0xff) << 8);
  } else {
    *(u16*)at = (*(u16*)at & 0xff00) | (byte & 0xff);
  }
}

static inline int DoubleTopByte(double* x) {
  char* top = (char*)x + 7;

  return Isd_ReadByte(top);
}

/* Isd_StoreByte without the byte mask, which the one use (a 7-bit value) does not need. Needed to match. */
static inline void SetDoubleTopByte(double* x, int value) {
  char* top = (char*)x + 7;

  if ((u32)top & 1) {
    *(u16*)(top - 1) = (*(u16*)(top - 1) & 0x00ff) | (value << 8);
  } else {
    *(u16*)top = (*(u16*)top & 0xff00) | value;
  }
}

/* The va_list is a pointer to the next argument word. */
#define ISD_VA_ARG(args, type) (*(args) += sizeof(type), ((type*)*(args))[-1])

/*
 * vprintf for the debugger: formats into a 64-byte buffer and passes the text to
 * target->write. Handles %% and the flags - + space # 0, width, precision and the
 * conversions s c d i u b o x X e E f g G. Length letters are skipped.
 */
void Isd_VFormat(IsdPrintTarget* target, const char* format, char** args) {
  char buf[64];
  char frac[64];
  u32 flags;
  int prec;
  int conversion;
  u32 radix;
  const char* str;
  int hexOffset;
  int bufLen;
  int width;
  int exponent;
  int c;
  int state;
  double scaled;
  char* out;
  int expPositive;
  int fieldPad;
  int fracDigits;
  int signLen;
  int sign;
  char* digit;
  double rounding;
  int charArg;
  int digitChar;
  int prefixTail;
  int prefixLen;
  int prefixHead;
  double fraction;
  int i;
  int fracLen;
  int droppedDigits;
  int trimZeros;
  int padding;
  int len;
  int count;
  u32 value;
  int isGeneral;
  double number;

  conversion = CONV_DECIMAL;
  width = -1;
  flags = 0;
  bufLen = 0;
  prec = -1;
  state = STATE_TEXT;
  hexOffset = 0;
  for (;;) {
    c = Isd_ReadByte(format);
    if (c == 0) {
      break;
    }
    format++;
    if (state == STATE_TEXT) {
      if (c == '%') {
        if (bufLen != 0) {
          target->write(target, buf, bufLen);
          bufLen = 0;
        }
        state = STATE_PERCENT;
        width = flags = 0;
        prec = -1;
      } else {
        Isd_StoreByte(&buf[bufLen], c);
        bufLen++;
        if (bufLen == 64) {
          target->write(target, buf, bufLen);
          bufLen = 0;
        }
      }
      continue;
    }

    switch ((u8)gIsdFormatActions[state - 1][(u8)gIsdFormatCharClass[c]]) {
      case STATE_PERCENT:
        Isd_StoreByte(&buf[bufLen], c);
        bufLen++;
        if (bufLen == 64) {
          target->write(target, buf, bufLen);
          bufLen = 0;
        }
        state = STATE_TEXT;
        break;
      case STATE_FLAGS:
        state = STATE_FLAGS;
        switch (c) {
          case '+':
            flags |= FORMAT_PLUS;
            break;
          case '-':
            flags |= FORMAT_LEFT;
            break;
          case ' ':
            flags |= FORMAT_SPACE;
            break;
          case '#':
            flags |= FORMAT_ALTERNATE;
            break;
          case '0':
            flags |= FORMAT_ZERO_PAD;
            break;
        }
        break;
      case STATE_WIDTH:
        state = STATE_WIDTH;
        if (width == -1) {
          width = c - '0';
        } else {
          width = width * 10 + (c - '0');
        }
        break;
      case STATE_DOT:
        state = STATE_DOT;
        break;
      case STATE_PRECISION:
        state = STATE_PRECISION;
        if (prec == -1) {
          prec = c - '0';
        } else {
          prec = prec * 10 + (c - '0');
        }
        break;
      case STATE_LENGTH:
        state = STATE_LENGTH;
        break;
      case STATE_CONVERSION:
        state = STATE_CONVERSION;
        switch (c) {
          case 'b':
            conversion = CONV_BINARY;
            break;
          case 'c':
            conversion = CONV_CHAR;
            break;
          case 's':
            conversion = CONV_STRING;
            break;
          case 'd':
          case 'i':
            conversion = CONV_DECIMAL;
            break;
          case 'u':
            conversion = CONV_UNSIGNED;
            break;
          case 'o':
            conversion = CONV_OCTAL;
            break;
          case 'x':
            conversion = CONV_HEX;
            break;
          case 'X':
            conversion = CONV_HEX_UPPER;
            break;
          case 'f':
            conversion = CONV_FIXED;
            break;
          case 'e':
            conversion = CONV_EXP;
            break;
          case 'E':
            conversion = CONV_EXP_UPPER;
            break;
          case 'g':
            conversion = CONV_GENERAL;
            break;
          case 'G':
            conversion = CONV_GENERAL_UPPER;
            break;
        }
        break;
    }

    if (state != STATE_CONVERSION) {
      continue;
    }

    switch (conversion) {
      case CONV_CHAR:
        charArg = ISD_VA_ARG(args, int);
        target->write(target, (char*)&charArg, 1);
        break;
      case CONV_STRING:
        str = ISD_VA_ARG(args, char*);
        if (str == NULL) {
          str = gIsdNullString;
        }
        len = Isd_StringLength(str);
        i = flags & FORMAT_LEFT;
        if (!i && len < width) {
          Isd_WriteSpaces(target, width - len);
        }
        target->write(target, str, len);
        if (i && len < width) {
          Isd_WriteSpaces(target, width - len);
        }
        break;
      case CONV_DECIMAL:
        value = ISD_VA_ARG(args, int);
        if ((int)value < 0) {
          flags |= FORMAT_NEGATIVE;
          value = -value;
        }
        flags |= FORMAT_SIGNED;
        radix = 10;
        goto integer;
      case CONV_UNSIGNED:
        value = ISD_VA_ARG(args, u32);
        flags |= FORMAT_UNSIGNED;
        radix = 10;
        goto integer;
      case CONV_BINARY:
        value = ISD_VA_ARG(args, u32);
        flags |= FORMAT_UNSIGNED;
        radix = 2;
        goto integer;
      case CONV_OCTAL:
        value = ISD_VA_ARG(args, u32);
        flags |= FORMAT_UNSIGNED;
        radix = 8;
        goto integer;
      case CONV_HEX:
        value = ISD_VA_ARG(args, u32);
        flags |= FORMAT_UNSIGNED;
        hexOffset = 'a' - '9' - 1;
        radix = 16;
        goto integer;
      case CONV_HEX_UPPER:
        value = ISD_VA_ARG(args, u32);
        flags |= FORMAT_UNSIGNED;
        radix = 16;
        hexOffset = 'A' - '9' - 1;
      integer:
        padding = width;
        if (padding > 60) {
          padding = 60;
        }
        out = &buf[63];
        if (prec == -1) {
          prec = 1;
        } else if (prec > 58) {
          prec = 58;
        }
        while (prec-- > 0 || value != 0) {
          u32 quotient = Isd_DivideByRadix(value, radix);
          digitChar = value - quotient * radix + '0';
          if ((u32)digitChar > '9') {
            digitChar += hexOffset;
          }
          value = quotient;
          Isd_StoreByte(out, digitChar);
          out--;
        }
        prefixLen = 0;
        if (radix != 10) {
          if (flags & FORMAT_ALTERNATE) {
            if (conversion == CONV_HEX) {
              prefixHead = '0';
              prefixTail = 'x';
              prefixLen = 2;
            } else if (conversion == CONV_HEX_UPPER) {
              prefixHead = '0';
              prefixTail = 'X';
              prefixLen = 2;
            } else if (conversion == CONV_OCTAL) {
              prefixHead = '0';
              prefixLen = 1;
            } else if (conversion == CONV_BINARY) {
              prefixHead = '0';
              prefixTail = 'b';
              prefixLen = 2;
            }
          }
        } else if (flags & FORMAT_NEGATIVE) {
          prefixHead = '-';
          prefixLen = 1;
        } else if (flags & FORMAT_PLUS) {
          prefixHead = '+';
          prefixLen = 1;
        } else if (flags & FORMAT_SPACE) {
          prefixHead = ' ';
          prefixLen = 1;
        }
        padding -= &buf[63] - out - prefixLen;
        if (padding > 0 && (flags & FORMAT_ZERO_PAD)) {
          /* Needed to match: the original tests the count again before the loop. */
          i = 0;
          if (i < padding) {
            for (; i < padding; i++) {
              Isd_StoreByte(out, '0');
              out--;
            }
          }
        }
        if (prefixLen != 0) {
          if (prefixLen == 2) {
            Isd_StoreByte(out, prefixTail);
            out--;
          }
          Isd_StoreByte(out, prefixHead);
          out--;
        }
        len = &buf[63] - out;
        if (padding > 0 && !(flags & (FORMAT_LEFT | FORMAT_ZERO_PAD))) {
          Isd_WriteSpaces(target, padding);
        }
        target->write(target, out + 1, len);
        if (padding > 0 && (flags & FORMAT_LEFT)) {
          Isd_WriteSpaces(target, padding);
        }
        break;
      case CONV_EXP:
      case CONV_EXP_UPPER:
      case CONV_FIXED:
      case CONV_GENERAL:
      case CONV_GENERAL_UPPER:
        number = ISD_VA_ARG(args, double);
        out = &buf[63];
        fieldPad = width;
        droppedDigits = sign = count = 0;
        trimZeros = 1;
        flags |= FORMAT_UNSIGNED;
        if (DoubleTopByte(&number) & 0x80) {
          flags |= FORMAT_NEGATIVE;
          SetDoubleTopByte(&number, DoubleTopByte(&number) & 0x7f);
        }
        scaled = number;
        if (width > 60) {
          fieldPad = 60;
        }
        if (Isd_DoubleExponent(scaled) < ISD_DOUBLE_EXPONENT_ONE) {
          for (exponent = 0;
               Isd_DoubleExponent(scaled) < ISD_DOUBLE_EXPONENT_ONE;
               exponent--) {
            scaled = Isd_DoubleTimesTen(scaled);
          }
        } else if (Isd_DoubleExponent(Isd_DoubleDivideByTen(scaled, 0)) >=
                   ISD_DOUBLE_EXPONENT_ONE) {
          for (exponent = 0; Isd_DoubleExponent(Isd_DoubleDivideByTen(
                                 scaled, 0)) >= ISD_DOUBLE_EXPONENT_ONE;
               exponent++) {
            scaled = Isd_DoubleDivideByTen(scaled, 1);
          }
        } else {
          exponent = 0;
        }
        if (conversion == CONV_FIXED && exponent > 58 - prec) {
          conversion = CONV_EXP;
        }
        if (conversion == CONV_GENERAL || conversion == CONV_GENERAL_UPPER) {
          isGeneral = 1;
        } else {
          isGeneral = 0;
        }
        if (prec == -1) {
          prec = 6;
        }
        if (prec > 58) {
          prec = 58;
        }
        if (conversion == CONV_GENERAL || conversion == CONV_GENERAL_UPPER) {
          if (exponent < -4 || exponent >= prec) {
            conversion =
                (conversion == CONV_GENERAL) ? CONV_EXP : CONV_EXP_UPPER;
          } else {
            conversion = CONV_FIXED;
          }
        }
        fracDigits = prec;
        if (isGeneral == 1) {
          if (prec == 0) {
            fracDigits = 1;
          }
          fracDigits -= (conversion == CONV_FIXED) ? exponent + 1 : 1;
        }
        if (conversion == CONV_EXP || conversion == CONV_EXP_UPPER) {
          number = scaled;
        }
        rounding = 5.0;
        for (i = 0; i <= fracDigits; i++) {
          rounding = Isd_DoubleDivideByTen(rounding, 1);
        }
        number = Isd_DoubleAdd(number, rounding);
        if (conversion == CONV_EXP || conversion == CONV_EXP_UPPER) {
          if (!isGeneral || exponent != 0) {
            if (exponent >= 0) {
              expPositive = 1;
            } else {
              exponent *= -1;
              expPositive = 0;
            }
            for (i = 0; i < 3; i++) {
              digitChar = exponent - Isd_DivideByRadix(exponent, 10) * 10 + '0';
              exponent = Isd_DivideByRadix(exponent, 10);
              Isd_StoreByte(out, digitChar);
              out--;
              count++;
            }
            Isd_StoreByte(out, expPositive ? '+' : '-');
            Isd_StoreByte(out - 1, conversion == CONV_EXP ? 'e' : 'E');
            out -= 2;
            count += 2;
          }
        }
        fraction = number;
        fracLen = 0;
        while (fracDigits > 0) {
          fraction = Isd_DoubleTimesTen(fraction);
          c = (u8)(Isd_DoubleUnitsDigit(fraction) + '0');
          digit = &frac[fracLen];
          Isd_StoreByte(digit, c);
          if (Isd_ReadByte(digit) < '0') {
            Isd_StoreByte(digit, '0');
          }
          fracLen++;
          fracDigits--;
        }
        while (fracLen > 0) {
          fracLen--;
          if (isGeneral == 1 && trimZeros &&
              Isd_ReadByte(&frac[fracLen]) == '0') {
            continue;
          }
          trimZeros = 0;
          Isd_StoreByte(out, Isd_ReadByte(&frac[fracLen]));
          out--;
          count++;
        }
        if (!trimZeros || (flags & FORMAT_ALTERNATE)) {
          Isd_StoreByte(out, '.');
          out--;
          count++;
        }
        while (exponent > 58 - prec) {
          droppedDigits++;
          number = Isd_DoubleDivideByTen(number, 1);
          exponent--;
        }
        c = (u8)(Isd_DoubleUnitsDigit(number) + '0');
        if (c < '0') {
          c = '0';
        }
        number = Isd_DoubleDivideByTen(number, 0);
        Isd_StoreByte(out, (u8)c); /* needed to match: c is already a byte */
        out--;
        count++;
        while (Isd_DoubleExponent(number) >= ISD_DOUBLE_EXPONENT_ONE) {
          c = (u8)(Isd_DoubleUnitsDigit(number) + '0');
          if (c < '0') {
            c = '0';
          }
          number = Isd_DoubleDivideByTen(number, 0);
          Isd_StoreByte(out, (u8)c);
          out--;
          count++;
        }
        signLen = 0;
        if (flags & FORMAT_NEGATIVE) {
          sign = '-';
          signLen = 1;
          count++;
        } else if (flags & FORMAT_PLUS) {
          sign = '+';
          signLen = 1;
          count++;
        } else if (flags & FORMAT_SPACE) {
          sign = ' ';
          signLen = 1;
          count++;
        }
        fieldPad -= count;
        if (fieldPad > 0 && (flags & FORMAT_ZERO_PAD)) {
          /* Needed to match: the original tests the count again before the loop. */
          i = 0;
          if (i < fieldPad) {
            for (; i < fieldPad; i++) {
              Isd_StoreByte(out, '0');
              out--;
            }
          }
        }
        if (signLen != 0) {
          Isd_StoreByte(out, sign);
          out--;
        }
        len = &buf[63] - out;
        if (fieldPad > 0 && !(flags & (FORMAT_LEFT | FORMAT_ZERO_PAD))) {
          Isd_WriteSpaces(target, fieldPad);
        }
        while (len > 0) {
          out++;
          if (Isd_ReadByte(out) == '.') {
            while (droppedDigits > 0) {
              target->write(target, gIsdZeroString, 1);
              droppedDigits--;
            }
          }
          target->write(target, out, 1);
          len--;
        }
        if (fieldPad > 0 && (flags & FORMAT_LEFT)) {
          Isd_WriteSpaces(target, fieldPad);
        }
        break;
    }
    state = STATE_TEXT;
  }
  if (bufLen != 0) {
    target->write(target, buf, bufLen);
  }
}
