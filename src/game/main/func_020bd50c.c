#include "types.h"

void func_0201e954(void);
void func_020e8848(void);

/* a0 and a1 are not used. Bit 1 of flags set: does nothing. */
void func_020bd50c(int a0, int a1, u32 flags) {
  if (flags & 2) {
    return;
  }
  func_0201e954();
  func_020e8848();
}
