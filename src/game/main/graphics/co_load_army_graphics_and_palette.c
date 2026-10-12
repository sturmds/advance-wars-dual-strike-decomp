#include "types.h"

void Co_LoadArmyGraphics(int coId, int tile, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);

extern u32 gLinkPlay;

/* Loads the picture of a CO into sprite tile memory (Co_LoadArmyGraphics)
 * and its palette into sprite palette slot paletteSlot (palette numbers
 * 16 and up are the sprite ones). Bit 7 of coId, or gLinkPlay being 0,
 * means army 0. */
void Co_LoadArmyGraphicsAndPalette(int coId, int tile, int paletteSlot,
                                   int army) {
  if (gLinkPlay == 0 || (coId & 0x80)) {
    army = 0;
  }
  Co_LoadArmyGraphics(coId, tile, army);
  Co_LoadArmyPalette(coId, paletteSlot + 16, army);
}
