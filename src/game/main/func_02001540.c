#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

void func_02001540(void) {
  Overlay_Unload(2);
  Overlay_Load(1);
}
