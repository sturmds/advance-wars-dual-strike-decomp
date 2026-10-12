#include "types.h"

extern int data_027c0284;
void func_020c4018(int, void*);

void func_0200a5e0(int a0) {
  data_027c0284 = a0;
  func_020c4018(a0, &data_027c0284);
}
