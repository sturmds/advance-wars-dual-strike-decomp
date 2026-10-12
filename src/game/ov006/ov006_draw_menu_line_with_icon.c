#include "types.h"
#include "game/Unk_02088d04.h"

void Icon_LoadTo(int iconId, u8* vram, int tile);
void IconBank_LoadPalette(int bank, int paletteSlot);
void Bg_PutTileRect(u16* map, int x, int y, int width, int height,
                    int firstTile, int palette);
void Ov006_DrawMenuLine(int unused, int row, const char* text, int indent);

extern Unk_027c0280* data_027c0280;

/* Loads icon 3 (palette slot 11) into the layer 0 character data, puts its 2x2
 * tiles at column 3 of menu row `row`, then prints text one column further right. */
void Ov006_DrawMenuLineWithIcon(int unused, int row, const char* text) {
  int y = 6;

  y += row * 2;
  Icon_LoadTo(
      3,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
      64);
  IconBank_LoadPalette(0, 11);
  Bg_PutTileRect(data_027c0280->layer0Map, 3, y, 2, 2, 64, 11);
  Ov006_DrawMenuLine(unused, row, text, 1);
}
