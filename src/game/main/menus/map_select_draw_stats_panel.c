#include "types.h"
#include "game/menu_state.h"

int MapSelect_GetTab(void);
void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile, int a4);
void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);
void MapSelect_DrawNumber(int x, int y, int value);

extern MenuState gMenuState;
extern u8 data_027c0020;
extern u8 data_027c0060;

#define MENU ((MenuState*)&gMenuState)

/* Draws the panel of a game mode 1 map list at x: a frame of two sprites,
 * then three rows with a sprite, two optional marker icons and two numbers.
 * Mode 0 reads the first set of setup fields (0x4da), any other mode the
 * second set (0x4ea). The name is a guess. */
void MapSelect_DrawStatsPanel(int x, int y, int mode) {
  int i;

  /* Needed to match: the original calls MapSelect_GetTab seven times here and
     * drops the result. */
  if (mode == 1) {
    MapSelect_GetTab();
    MapSelect_GetTab();
    MapSelect_GetTab();
    MapSelect_GetTab();
    MapSelect_GetTab();
    MapSelect_GetTab();
    MapSelect_GetTab();
  }
  if (x < -128) {
    return;
  }
  if (x > 256) {
    return;
  }
  Sprite_QueueAt(0, (x + 64) & 0x1ff, y & 0xff, &data_027c0020, 0x1000, 0x196);
  Sprite_QueueAt(0, (x + 96) & 0x1ff, y & 0xff, &data_027c0020, 0x1000, 0x19a);
  for (i = 0; i < 3; i++) {
    Sprite_QueueAt(0, (x + 8) & 0x1ff, (y + 8 + i * 32) & 0xff, &data_027c0060,
                   0x1000, 0x19e + i * 8);
    if (mode == 0) {
      if (MENU->setup.recordDays[i] != 0) {
        if (MENU->setup.recordCos[i * 2] != 0) {
          Co_QueueSmallPictureSpriteAtTile(
              (x + 32) & 0x1ff, (y + 40 + i * 32) & 0xff, (i * 2 + 3) << 12,
              (i * 2) * 12 + 0x1ca, ((i * 2) & 1) + 1);
        }
        if (MENU->setup.recordCos[i * 2 + 1] != 0) {
          Co_QueueSmallPictureSpriteAtTile(
              (x + 64) & 0x1ff, (y + 40 + i * 32) & 0xff, (i * 2 + 4) << 12,
              (i * 2 + 1) * 12 + 0x1ca, ((i * 2 + 1) & 1) + 1);
        }
        MapSelect_DrawNumber(x + 64, y + 16 + i * 32,
                             MENU->setup.recordDays[i]);
        MapSelect_DrawNumber(x + 96, y + 16 + i * 32,
                             MENU->setup.recordScores[i]);
      }
    } else {
      if (MENU->setup.prevRecordDays[i] != 0) {
        if (MENU->setup.prevRecordCos[i * 2] != 0) {
          Co_QueueSmallPictureSpriteAtTile(
              (x + 32) & 0x1ff, (y + 40 + i * 32) & 0xff, (i * 2 + 9) << 12,
              (i * 2 + 6) * 12 + 0x1ca, ((i * 2 + 6) & 1) + 1);
        }
        if (MENU->setup.prevRecordCos[i * 2 + 1] != 0) {
          Co_QueueSmallPictureSpriteAtTile(
              (x + 64) & 0x1ff, (y + 40 + i * 32) & 0xff, (i * 2 + 10) << 12,
              (i * 2 + 7) * 12 + 0x1ca, ((i * 2 + 7) & 1) + 1);
        }
        MapSelect_DrawNumber(x + 64, y + 16 + i * 32,
                             MENU->setup.prevRecordDays[i]);
        MapSelect_DrawNumber(x + 96, y + 16 + i * 32,
                             MENU->setup.prevRecordScores[i]);
      }
    }
  }
}
