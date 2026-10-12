#include "types.h"
#include "sdk/msl.h"

void __round_decimal(Decimal* dec, int newLength) {
  u8* p;
  u8* tail;
  u32 digit;
  u32 carry;
  u8* digits;
  int length;

  if (newLength < 0) {
  zero: /* shared with the all-digits-rounded-away case below; needed to match */
    dec->exp = 0;
    dec->length = 1;
    dec->digits[0] = '0';
    return;
  }
  length = dec->length;
  if (newLength >= length) {
    return;
  }

  digits = dec->digits;
  p = digits + newLength + 1;
  digit = (u8)(*--p - '0');
  if (digit == 5) {
    tail = digits + length;
    do {
      tail--;
    } while (tail > p && *tail == '0');
    if (tail == p) {
      carry = p[-1] & 1;
    } else {
      carry = 1;
    }
  } else {
    carry = digit > 5;
  }

  while (newLength != 0) {
    digit = (u8)(*--p - '0' + carry);
    carry = digit > 9;
    if (carry || digit == 0) {
      newLength--;
    } else {
      *p = digit + '0';
      break;
    }
  }

  if (carry) {
    dec->exp++;
    dec->length = 1;
    dec->digits[0] = '1';
    return;
  }
  if (newLength == 0) {
    goto zero;
  }
  dec->length = newLength;
}
