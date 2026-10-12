#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bb90.h"

/* Needed to match: the original keeps col * 2 and the row sums in the loops. */
#pragma opt_strength_reduction off
#pragma opt_loop_invariants off

void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);

extern u32 data_ov005_02367f8c[];
extern u32 data_ov005_02367f9c[];

/* Moves the staff credits background one pixel right every 10 frames (wrapping from
 * 0x500 back to 0x100), copies the visible 33 x 8 tile window of work->palettesOut
 * (192 tiles wide) into layer 2's map, and sets the layer's scroll. */
void Ov005_ScrollStaffCreditsBackground(Unk_ov005_0236bb90* work) {
  int row;
  int col;
  Unk_027c0280* saved0280;
  u32 rowBase;
  u16* src;
  int rowOffset;
  Unk_02080440* saved027c;
  int scrollY;
  u16* map;
  int scrollX;
  int mapRow;

  if (work->unk_835c == 10) {
    work->unk_835c = 0;
    work->unk_834c++;
    if (work->unk_834c >= 0x500) {
      work->unk_834c -= 0x400;
    }
  }
  work->unk_835c++;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);

  scrollX = work->unk_834c;
  scrollY = work->unk_8350;
  work->unk_8354 = scrollX & 0x1ff;
  work->unk_8358 = scrollY & 0xff;
  rowBase = (scrollY >> 3) * 192 + (scrollX >> 3);
  for (row = 0, rowOffset = 0; row < 8; row++) {
    mapRow = (((scrollY >> 3) + row) & 31) << 5;
    src = (u16*)work->palettesOut;

    for (col = 0; col < 33; col++) {
      int page = 0;
      int tileX = (scrollX >> 3) + col;
      int index;

      if (tileX & 32) {
        page += 0x400;
      }
      index = page + (tileX & 31);
      index += mapRow;
      (data_027c0280->layer2Map + index)[0x200] =
          (src + rowBase + rowOffset)[col];
    }
    rowOffset += 192;
  }

  map = data_027c0280->layer2Map;
  TileMap_AddToEntries(
      map,
      Screen_IsTextBg(2)
          ? data_ov005_02367f8c[GFX_LAYER(data_027c0280, 2)->screenSize]
      : Screen_IsAffineBg(2)
          ? data_ov005_02367f9c[GFX_LAYER(data_027c0280, 2)->screenSize]
          : data_ov005_02367f9c[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
      0);
  Gfx_RequestLayerCopy(2);
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = work->unk_8354;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = work->unk_8358;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
