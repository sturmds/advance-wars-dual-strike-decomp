#include "types.h"

void ModeFont_ShowSecondLine(const char* text, int tile, int paletteSlot,
                             int unk_56, u8 unk_5c, int unk_58);

void func_0202a7ec(const char* text, int tile, int paletteSlot, u8 unk_5c,
                   int unk_58) {
  ModeFont_ShowSecondLine(text, tile, paletteSlot, 0, unk_5c, unk_58);
}
