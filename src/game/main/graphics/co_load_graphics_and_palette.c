#include "types.h"

void Co_LoadGraphics(int coId, int firstTile);
void Co_LoadPalette(int coId, int paletteSlot);

/* Loads the picture of a CO into sprite tile memory (Co_LoadGraphics) and
 * its palette into sprite palette slot paletteSlot (palette numbers 16 and
 * up are the sprite ones). */
void Co_LoadGraphicsAndPalette(int coId, int tile, int paletteSlot) {
  Co_LoadGraphics(coId, tile);
  Co_LoadPalette(coId, paletteSlot + 16);
}
