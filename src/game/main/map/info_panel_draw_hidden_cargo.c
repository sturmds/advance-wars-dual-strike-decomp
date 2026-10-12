#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
extern void* data_021663c8[];
extern u8 gInfoPanelHiddenCargoMark[];

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);
void TransferQueue_Add(const void* src, void* dst, u32 size);

/* Draws the mark shown for one seat of a transport whose cargo is hidden: a
 * 16x16 sprite in palette 12 whose 4 tiles (128 bytes) are queued to OBJ VRAM
 * at tile 840 + slot * 4. slot is 1 or 2. */
void InfoPanel_DrawHiddenCargoMark(s16 x, s16 y, int slot) {
  int tile = slot * 4;

  Oam_PutSprite(x, y | 0x400, data_021663c8[1], tile | 0xc000);
  TransferQueue_Add(gInfoPanelHiddenCargoMark,
                    data_027c0280->objVram + (tile + 840) * 32, 0x80);
}
