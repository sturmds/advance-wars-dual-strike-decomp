#include "types.h"

void NumberSprite_PutDigits(int x, int y, int value, int tile);
void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

extern u8 data_027c0048[];

/* Draws value with NumberSprite_PutDigits, then one more sprite 24 pixels left of x and
 * 8 above y. Its tile is the one after the ten digit tiles, which sit 2 apart
 * from tile + 0x1000. InfoPanel_Draw reaches it through
 * InfoPanel_DrawDamageForecast. What the extra sprite shows is not known. */
void func_020d2df0(int x, int y, int value, int tile) {
  NumberSprite_PutDigits(x, y, value, tile);
  Oam_PutSprite((x - 24) & 0x1ff, (y - 8) & 0xff, data_027c0048, tile + 0x1014);
}
