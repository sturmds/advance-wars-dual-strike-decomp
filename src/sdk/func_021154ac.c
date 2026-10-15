#include "types.h"
#include "sdk/os.h"

int func_021154d8(void);

void func_021154ac(void) {
  if (func_021154d8() != 0) {
    return;
  }
  OS_Terminate();
}
