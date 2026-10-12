#include "types.h"

int Link_IsActive(void);
void func_0207b4e8(void);

/* In link mode (Link_IsActive returns 1), runs func_0207b4e8: link state 2 to
 * 3. */
void LinkMode_AdvanceState(void) {
  if (Link_IsActive() != 1) {
    return;
  }
  func_0207b4e8();
}
