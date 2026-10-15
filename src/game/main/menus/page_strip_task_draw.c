#include "types.h"
#include "game/player.h"
#include "game/menu_state.h"
#include "game/page_strip.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/screen_other_map.h"

u32 HwMod32(int num, int denom);
int MainLoop_GetFrameCount(void);
int Map_GetPlayerCount(int mapId);
void Palette_Load(const void* src, int offset, int size);
void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);
void Sprite_QueuePicture192x160(s16 x, int y, int a2, int a3, int a4);
void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile, int a4);
void func_020485f0(u8 pages);

/* Draws one frame of a page strip on the top screen, the same way as
 * SurvivalSelect_Draw but from the task's fields and with palettes 6 and 7:
 * one tab per entry (the selected one 32 pixels wide, the others 16) with a
 * link piece between tabs, one marker per player for the map's player count,
 * and two palette colours that cycle every 4 frames. */
void PageStripTask_Draw(PageStripTask* task) {
  int playerCount = Map_GetPlayerCount(task->mapId);
  Unk_027c0280* savedState = data_027c0280;
  int col;
  int x;
  int i;
  int j;
  int pal;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  x = 0;
  col = 0;
  for (i = 0; i < task->count; i++) {
    pal = 6;
    if (task->pages >= 2) {
      if (i < HwMod32(task->unk_66, task->count)) {
        pal = 7;
      } else {
        pal = 6; /* needed to match: makes mwcc load 6 before 7 */
      }
    }
    if (i == task->cursor) {
      Sprite_QueueAt(6, x & 0x1ff, 0, &data_027c0048, (pal << 12) | 0x800,
                     col * 16 + 90);
      x += 32;
    } else {
      Sprite_QueueAt(6, x & 0x1ff, 8, &data_027c0040, (pal << 12) | 0x800,
                     0x14a + col * 4);
      x += 16;
    }
    if (task->pages >= 2) {
      if (i < HwMod32(task->unk_66 - 1, task->count)) {
        pal = 7;
      } else {
        pal = 6;
      }
    }
    if (i != task->count - 1) {
      Sprite_QueueAt(5, (x - 3) & 0x1ff, 16, &data_027c0028,
                     (pal << 12) | 0x800, 88);
      col++;
      if (col == 15) {
        col = 0;
      }
    }
  }
  func_020485f0(task->pages);
  for (i = 0; i < playerCount - 1; i++) {
    /* Needed to match: the bound is written as <= count - 1. */
    for (j = 0; j <= PLAYER_CO_COUNT - 1; j++) {
      int k = j + i * 2;
      Co_QueueSmallPictureSpriteAtTile(
          (24 + j * 32) & 0x1ff, (192 - i * 16) & 0xff, k << 12, k * 12, k & 1);
    }
  }
  Sprite_QueueAt(0, 0, (192 - playerCount * 16) & 0xff, &data_027c00b8, 0x8000,
                 72);
  Sprite_QueuePicture192x160(128, 96, 0x9800, 0x400, 7);
  Palette_Load(&gMenuState.colours[(MainLoop_GetFrameCount() >> 2) & 15], 0x2d8,
               2);
  Palette_Load(&gMenuState.colours[(MainLoop_GetFrameCount() >> 2) & 15], 0x2f8,
               2);
  Screen_SetCurrentGfx(savedState);
}
