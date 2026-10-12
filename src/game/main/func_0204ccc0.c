#include "types.h"

void Sound_PlaySe(u32 soundId);
int func_0204cd00(void);
void MenuButtons_CloseAll(void);

void func_0204ccc0(void) {
  Sound_PlaySe(0x10006);
  func_0204cd00();
  MenuButtons_CloseAll();
}
