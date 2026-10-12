#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

void func_020015f4(void) {
  Overlay_Unload(1);
  Overlay_Load(5);
}
