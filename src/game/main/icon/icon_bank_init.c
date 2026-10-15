#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/icon.h"

/* Empties icon bank bank, gives it the VRAM area vram from tile number
 * firstTile on and palette slot paletteSlot, and loads its palette there. */
void IconBank_Init(int bank, u8* vram, int firstTile, int paletteSlot) {
  data_027c0280->iconBanks[bank].vram = vram;
  data_027c0280->iconBanks[bank].palette = paletteSlot;
  data_027c0280->iconBanks[bank].count = 0;
  data_027c0280->iconBanks[bank].slots[0].tile = firstTile;
  IconBank_LoadPalette(bank, paletteSlot);
}
