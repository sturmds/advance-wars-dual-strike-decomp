#include "types.h"
#include "game/screen_other_map.h"

void Palette_QueueBlockCopy(int id, int army, void* dest, int copy);

/* Loads terrain graphics 8 for armies 1 to 4 into OBJ VRAM of the second screen, 0x100 bytes apart from 0x3800. */
void Ov006_LoadArmyTerrainGraphics(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  Unk_027c0280* savedOuter;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  savedOuter = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Palette_QueueBlockCopy(8, 1, data_027c0280->objVram + 0x3800, 1);
  Palette_QueueBlockCopy(8, 2, data_027c0280->objVram + 0x3900, 1);
  Palette_QueueBlockCopy(8, 3, data_027c0280->objVram + 0x3a00, 1);
  Palette_QueueBlockCopy(8, 4, data_027c0280->objVram + 0x3b00, 1);
  Screen_SetCurrentGfx(savedOuter);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
