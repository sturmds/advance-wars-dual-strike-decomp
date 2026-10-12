#include "types.h"

void PopupMenu_PushState(void);
void func_0201e954(void);
void func_020b9e04(void);
void func_020739a8(void);

/* Menu callback with the same signature as func_020bd50c: a0 and a1 are not
 * used, and flags with bit 1 set make it do nothing. */
void func_020bd5f4(int a0, int a1, u32 flags) {
  if (flags & 2) {
    return;
  }
  PopupMenu_PushState();
  func_0201e954();
  func_020b9e04();
  func_020739a8();
}
