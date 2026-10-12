#include "types.h"

void LinkMenu_PushSelectEntry(int a0);
void LinkMenu_PushLower(void);

void func_02087714(u8* a0) {
  LinkMenu_PushSelectEntry((*a0 - 8) & 255);
  LinkMenu_PushLower();
}
