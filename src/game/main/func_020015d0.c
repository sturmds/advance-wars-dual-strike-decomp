#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

void func_020015d0(void) {
  Overlay_Unload(5);
  Overlay_Load(1);
}
