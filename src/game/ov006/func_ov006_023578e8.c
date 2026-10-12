#include "types.h"

void PopupMenu_PopState(void);
void Sound_PlaySe(u32 soundId);

void func_ov006_023578e8(void) {
  PopupMenu_PopState();
  Sound_PlaySe(0x10009);
}
