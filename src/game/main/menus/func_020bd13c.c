#include "types.h"

extern u8 data_022a6b30;

void func_020ce158(int a0);
void func_0201e954(void);
void MenuDepth_Push(void);

/* Unit command menu handler (table entry at 0x02166140). Sets data_022a6b30,
 * which UnloadTask_PickTile and MapCursor_SelectTile read; its meaning is not known. */
void func_020bd13c(void) {
  data_022a6b30 = 1;
  func_020ce158(1);
  func_0201e954();
  MenuDepth_Push();
}
