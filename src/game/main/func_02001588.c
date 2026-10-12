#include "types.h"

void Overlay_Unload(int slot);
void Overlay_Load(int slot);

void func_02001588(void) {
  Overlay_Unload(6);
  Overlay_Load(1);
}
