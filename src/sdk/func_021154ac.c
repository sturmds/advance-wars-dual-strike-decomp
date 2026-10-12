#include "types.h"

int func_021154d8(void);
void OS_Terminate(void);

void func_021154ac(void) {
  if (func_021154d8() != 0) {
    return;
  }
  OS_Terminate();
}
