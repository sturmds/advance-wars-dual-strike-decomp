#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

void func_020014f8(void) {
  Overlay_Unload(3);
  Overlay_Load(1);
}
