#include "types.h"

int func_020f21fc(void);
void func_020f2210(int value);

/* Lowers the value read by func_020f21fc (Unk_021694c0 field 0x14) by `amount`, stopping at 0. */
void func_020f2224(int amount) {
  if (func_020f21fc() > amount) {
    func_020f2210(func_020f21fc() - amount);
  } else {
    func_020f2210(0);
  }
}
