#include "types.h"
#include "game/BgControl.h"

/* charOffset is a byte offset into BG VRAM; the register holds it in 16 KB steps. */
void BgControl_SetCharBase(BgControl* bg, u32 charOffset) {
  bg->charBase = charOffset >> 14;
}
