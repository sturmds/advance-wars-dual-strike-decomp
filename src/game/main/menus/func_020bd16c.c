#include "types.h"

extern u8 data_022a6b30;
extern u8 data_022a6b34[];

void func_020ce158(int a0);
void func_0201e954(void);
void MenuDepth_Push(void);

/* Unit command menu handler (table entry at 0x02166120). Copies data_022a6b34[0]
 * as 0 or 1 into data_022a6b30; meaning not known. */
void func_020bd16c(void) {
  if (data_022a6b34[0] != 0) {
    data_022a6b30 = 1;
  } else {
    data_022a6b30 = 0;
  }
  func_020ce158(0);
  func_0201e954();
  MenuDepth_Push();
}
