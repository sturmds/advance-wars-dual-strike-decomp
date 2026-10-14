#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

int Co_GetNation(int co);
s32 HwMod32(s32 numerator, s32 denominator);
void Sprite_QueueAt(int list, int x, int y, const void* data, int unk08,
                    int unk0a);
void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile,
                                      int list);
void Sprite_QueueDefaultTile(int x, int y, int attr, int tile, int list);
void CombatSetup_DrawRowPlayerIcons(int x, int y);
void LinkMenu_DrawSetting(int a0, int a1, int a2, int a3);

extern MenuState gMenuState;
extern u8 data_02160ae0[];
extern UnitCountTable data_02160afc;
extern u8 data_021617ec;
extern u8 data_021617f4;
extern u8 data_021617fc;
extern u8 data_02161818;
extern u8 data_02161834;
extern u8 data_02161850;
extern u8 data_02161870;
extern u8 data_02161890;
extern u8 data_027c0028;
extern u8 data_027c0040;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Queues the sprites of the Combat setup screen for the local player's row
 * (link play). Main screen: the row's picture and CO, rows of markers from
 * data_02160afc for the row's kind (buttonId - 22), a sprite picked by kind
 * and the CO's nation (1 or other), four unit icons, then one icon per cell
 * of the row, four to a line. Sub screen: a sprite for each row with memberCount
 * set, and five LinkMenu_DrawSetting entries.
 * Needed to match: one variable, type, counts the marker lines and also
 * holds each cell's icon index. */
void CombatSetup_DrawRowSprites(void) {
  void* kindSprite;
  Unk_027c0280* subSaved0280;
  u8 kind;
  u16 x;
  Unk_02080440* subSaved027c;
  u8 row;
  u16 y;
  int i;
  int type;
  u8 nation;
  Unk_027c0280* saved0280;
  u16 tiles[4];
  UnitCountTable table;
  Unk_02080440* saved027c;
  table = data_02160afc;

  row = MENU->combat.players[data_0229fb30.unk_45a].row;
  kind = MENU->combat.rows[row - 1].buttonId - 22;
  nation = Co_GetNation(data_02160ae0[LINK_ROWS[row - 1].unk_19]);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Co_QueueSmallPictureSpriteAtTile(0x10e0, 24, (row + 1) << 12,
                                   (row - 1) * 12 + 0x490, 0);
  Sprite_QueueDefaultTile(0x8c, 8, 0xb000,
                          LINK_ROWS[row - 1].unk_19 * 12 + 0x4c0, 0);
  for (type = 0; type < 4; type++) {
    for (i = 0; i < table.counts[kind * 4 + type]; i++) {
      Sprite_QueueAt(0, (88 + i * 8) & 0x1ff, (92 + type * 16) & 0xff,
                     &data_027c0028, 0xa000, 0x638);
    }
  }
  if (nation == 1) {
    switch (kind) {
      case 0:
        kindSprite = &data_021617ec;
        break;
      case 1:
        kindSprite = &data_02161850;
        break;
      case 2:
        kindSprite = &data_02161870;
        break;
      case 3:
        kindSprite = &data_02161890;
        break;
    }
  } else {
    switch (kind) {
      case 0:
        kindSprite = &data_021617f4;
        break;
      case 1:
        kindSprite = &data_021617fc;
        break;
      case 2:
        kindSprite = &data_02161818;
        break;
      case 3:
        kindSprite = &data_02161834;
        break;
    }
  }
  Sprite_QueueAt(1, 8, 88, kindSprite, 0x6400, 0x514);
  for (i = 0; i < 4; i++) {
    tiles[i] = 0x624 + i * 4;
  }
  if (nation == 5) {
    tiles[0] += 16;
  }
  for (i = 0; i < 4; i++) {
    Sprite_QueueAt(2, 7, (24 + i * 16) & 0xff, &data_027c0040, 0x9400,
                   tiles[i]);
  }
  x = 127;
  y = 24;
  for (i = 0; i < LINK_ROWS[row - 1].spawnCount; i++) {
    switch (LINK_ROWS[row - 1].cells[i].tile) {
      case 1:
      case 2:
        type = 0;
        break;
      case 3:
        type = 1;
        break;
      case 4:
        type = 2;
        break;
      case 5:
        type = 3;
        break;
    }
    Sprite_QueueAt(2, x & 0x1ff, y & 0xff, &data_027c0040, 0x9400, tiles[type]);
    x += 24;
    if (HwMod32(i, 4) == 3) {
      x -= 96;
      y += 16;
    }
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  subSaved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  subSaved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  CombatSetup_DrawRowPlayerIcons(48, 48);
  for (i = 0; i < 4; i++) {
    if (MENU->combat.rows[i].memberCount != 0) {
      Co_QueueSmallPictureSpriteAtTile(80, (64 + i * 32) & 0xff, (i + 4) << 12,
                                       300 + i * 12, 4);
    }
  }
  for (i = 0; i < 5; i++) {
    LinkMenu_DrawSetting(16, 16 + i * 32, i, 0);
  }
  Screen_SetCurrent(subSaved027c);
  Screen_SetCurrentGfx(subSaved0280);
}
