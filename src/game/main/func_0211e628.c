#include "types.h"

int Cartridge_SendWordUntilTaken(int);
int OS_Terminate(int);

int func_0211e628(void) {
  int t0 = Cartridge_SendWordUntilTaken(2);
  return OS_Terminate(t0);
}
