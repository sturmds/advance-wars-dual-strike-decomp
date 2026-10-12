#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

void func_02001564(void) {
  Overlay_Unload(1);
  Overlay_Load(2);
}
