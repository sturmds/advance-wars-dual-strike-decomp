#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

void func_020015ac(void) {
  Overlay_Unload(1);
  Overlay_Load(6);
}
