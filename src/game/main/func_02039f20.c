#include "types.h"

int func_02039f60(int ch);

int func_02039f20(const u8* str) {
  int width = 0;

  while (*str != 0) {
    width += func_02039f60(*str);
    str++;
  }
  return width;
}
