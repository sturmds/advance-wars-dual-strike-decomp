#include "types.h"

void Sound_PlaySe(int se);
void PopupMenu_PopState(void);
void Ov006_PostRequest(int request);

void func_ov006_023543e8(void) {
  Sound_PlaySe(0x10009);
  PopupMenu_PopState();
  Ov006_PostRequest(5);
}
