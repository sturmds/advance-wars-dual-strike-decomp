#include "types.h"

extern u8 data_022a6b34[];

void func_0201e954(void);

/* Listed in a data table at 0x02166124. Calls func_0201e954 only when both
 * bytes of data_022a6b34 are 0. What the two bytes mean is unknown. */
void func_0201ecac(void) {
  if (data_022a6b34[0] + data_022a6b34[1] != 0) {
    return;
  }
  func_0201e954();
}
