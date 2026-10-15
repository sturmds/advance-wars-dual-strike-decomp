#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02160ac0.h"
#include "game/oam_cells.h"
#include "game/math.h"
#include "game/menu_button.h"

int Co_GetNation(int co);
void Sprite_QueueAt(int list, int x, int y, const void* data, int attr,
                    int tile);
void Sprite_QueueDefault(int x, int y, int attr, int list);
void Co_QueueSmallPictureSprite(int x, int y, int attr, int list);

extern u8
    gCombatModeCos[]; /* CO ids: Andy, Max, Sami, Flak, Lash, Adder, Hawke */
extern IconCounts gCombatModeIconCounts;
extern u8 data_021617ec[];
extern u8 data_021617f4[];
extern u8 data_021617fc[];
extern u8 data_02161818[];
extern u8 data_02161834[];
extern u8 data_02161850[];
extern u8 data_02161870[];
extern u8 data_02161890[];

/* Draws the sprites of the Combat mode menu (entries 8 to 11): a sprite for
 * the CO that
 * data_02160ac0->co picks from gCombatModeCos, four rows of icons whose
 * counts depend on the selected entry, a picture for the entry (one set for
 * Orange Star COs, another for the rest), a column of four icons (the first
 * moved by 16 tiles for Black Hole), and one of those icons for each of the
 * unk_18 cells of data_02160ac0, four per line. Cell tiles 1 and 2 share the
 * first icon. */
void CombatModeMenu_DrawSprites(void) {
  void* picture;
  u16 cellX;
  u8 selected;
  IconCounts counts;
  int i;
  u16 kindTiles[4];
  u8 nation;
  int row;
  int y;
  u16 cellY;
  counts = gCombatModeIconCounts;

  selected = Menu_GetSelectedId() - 8;
  nation = Co_GetNation(gCombatModeCos[data_02160ac0->co]);
  Co_QueueSmallPictureSprite(
      0x10e0, 24, data_02160ac0->co * 12 | data_02160ac0->co << 12, 0);
  Sprite_QueueDefault(140, 8, 0x7054, 0);
  y = 92;
  for (row = 0; row < (int)ARRAY_COUNT(kindTiles); row++) {
    for (i = 0; i < counts.n[selected * 4 + row]; i++) {
      Sprite_QueueAt(0, (88 + i * 8) & 0x1ff, y & 0xff, &data_027c0028, 0x8060,
                     0);
    }
    y += 16;
  }
  if (nation == NATION_ORANGE_STAR) {
    switch (selected) {
      case 0:
        picture = data_021617ec;
        break;
      case 1:
        picture = data_02161850;
        break;
      case 2:
        picture = data_02161870;
        break;
      case 3:
        picture = data_02161890;
        break;
    }
  } else {
    switch (selected) {
      case 0:
        picture = data_021617f4;
        break;
      case 1:
        picture = data_021617fc;
        break;
      case 2:
        picture = data_02161818;
        break;
      case 3:
        picture = data_02161834;
        break;
    }
  }
  Sprite_QueueAt(1, 8, 88, picture, 0x9400, 0x400);
  for (i = 0; i < (int)ARRAY_COUNT(kindTiles); i++) {
    kindTiles[i] = 0x510 + i * 4;
  }
  if (nation == NATION_BLACK_HOLE) {
    kindTiles[0] += 16;
  }
  for (i = 0; i < (int)ARRAY_COUNT(kindTiles); i++) {
    Sprite_QueueAt(2, 7, (24 + i * 16) & 0xff, &data_027c0040, 0xc400,
                   kindTiles[i]);
  }
  cellX = 127;
  cellY = 24;
  for (i = 0; i < data_02160ac0->cellCount; i++) {
    /* Needed to match: row doubles as the icon index. Tile 0 or a tile
         * above 5 keeps the previous index (4, past the end, on the first
         * cell). */
    switch (data_02160ac0->cells[i].tile) {
      case 1:
      case 2:
        row = 0;
        break;
      case 3:
        row = 1;
        break;
      case 4:
        row = 2;
        break;
      case 5:
        row = 3;
        break;
    }
    Sprite_QueueAt(2, cellX & 0x1ff, cellY & 0xff, &data_027c0040, 0xc400,
                   kindTiles[row]);
    cellX += 24;
    if (HwMod32(i, 4) == 3) {
      cellX -= 96;
      cellY += 16;
    }
  }
}
