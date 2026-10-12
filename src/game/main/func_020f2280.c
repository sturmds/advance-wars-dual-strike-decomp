#include "types.h"

int HwDiv32(int numerator, int denominator);
int func_020f225c(void);
int func_020f21fc(void);
int func_020f21d4(void);

int func_020f2280(void) {
  int value;

  if (func_020f225c() == 0) {
    return 0;
  }
  value = func_020f21fc();
  return HwDiv32(value * 100, func_020f21d4()) <= 30;
}
