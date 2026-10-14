#include "types.h"
#include "sdk/msl.h"

/* "NAN" */
char data_0216df28[4] = {
#include "data/main/data_0216df28.inc.c"
};
/* "nan" */
char data_0216df2c[4] = {
#include "data/main/data_0216df2c.inc.c"
};
/* "0x0p0" */
char data_0216df30[6] = {
#include "data/main/data_0216df30.inc.c"
};
/* Digits of exact powers of two. */
/* 2^-64 */
static char data_0216df38[46] = {
#include "data/main/data_0216df38.inc.c"
};
extern const char data_0216df68[]; /* 2^-53 */
extern const char data_0216df90[]; /* 2^-32 */
extern const char data_0216dfa8[]; /* 2^-16 */
extern const char data_0216dfb8[]; /* 2^-8 */
extern const char data_0216dfc0[]; /* 2^-7 */
extern const char data_0216dfc8[]; /* 2^-6 */
extern const char data_0216dfd0[]; /* 2^-5 */
extern const char data_0216dfd8[]; /* 2^-4 */
extern const char data_0216dfdc[]; /* 2^-3 */
extern const char data_0216dfe0[]; /* 2^-2 */
extern const char data_0216dfe4[]; /* 2^-1 */
extern const char data_0216dfe8[]; /* 2^0 */
extern const char data_0216dfec[]; /* 2^1 */
extern const char data_0216dff0[]; /* 2^2 */
extern const char data_0216dff4[]; /* 2^3 */
extern const char data_0216dff8[]; /* 2^4 */
extern const char data_0216dffc[]; /* 2^5 */
extern const char data_0216e000[]; /* 2^6 */
extern const char data_0216e004[]; /* 2^7 */
extern const char data_0216e008[]; /* 2^8 */

void __two_exp(Decimal* dec, long exponent) {
  switch (exponent) {
    case -64:
      __str2dec(dec, data_0216df38, -20);
      return;
    case -53:
      __str2dec(dec, data_0216df68, -16);
      return;
    case -32:
      __str2dec(dec, data_0216df90, -10);
      return;
    case -16:
      __str2dec(dec, data_0216dfa8, -5);
      return;
    case -8:
      __str2dec(dec, data_0216dfb8, -3);
      return;
    case -7:
      __str2dec(dec, data_0216dfc0, -3);
      return;
    case -6:
      __str2dec(dec, data_0216dfc8, -2);
      return;
    case -5:
      __str2dec(dec, data_0216dfd0, -2);
      return;
    case -4:
      __str2dec(dec, data_0216dfd8, -2);
      return;
    case -3:
      __str2dec(dec, data_0216dfdc, -1);
      return;
    case -2:
      __str2dec(dec, data_0216dfe0, -1);
      return;
    case -1:
      __str2dec(dec, data_0216dfe4, -1);
      return;
    case 0:
      __str2dec(dec, data_0216dfe8, 0);
      return;
    case 1:
      __str2dec(dec, data_0216dfec, 0);
      return;
    case 2:
      __str2dec(dec, data_0216dff0, 0);
      return;
    case 3:
      __str2dec(dec, data_0216dff4, 0);
      return;
    case 4:
      __str2dec(dec, data_0216dff8, 1);
      return;
    case 5:
      __str2dec(dec, data_0216dffc, 1);
      return;
    case 6:
      __str2dec(dec, data_0216e000, 1);
      return;
    case 7:
      __str2dec(dec, data_0216e004, 2);
      return;
    case 8:
      __str2dec(dec, data_0216e008, 2);
      return;
  }
  {
    Decimal root;
    Decimal square;

    /* exponent / 2 rounded toward zero, written out: needed to match the and of the sign bit */
    __two_exp(&root,
              (exponent + (long)((u32)(exponent & 0x80000000) >> 31)) >> 1);
    __timesdec(dec, &root, &root);
    /* Odd exponent: one more factor of 2, or of 1/2 when negative. */
    if (exponent & 1) {
      square = *dec;
      if (exponent > 0) {
        __str2dec(&root, data_0216dfec, 0);
      } else {
        __str2dec(&root, data_0216dfe4, -1);
      }
      __timesdec(dec, &square, &root);
    }
  }
}
