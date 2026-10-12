#include "types.h"

void LinkMenu_PushSelectEntry(int a0);
void LinkMenu_PushRaise(void);

void func_0208773c(u8* a0) {
  LinkMenu_PushSelectEntry((*a0 - 4) & 255);
  LinkMenu_PushRaise();
}
