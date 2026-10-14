#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"
#include "game/tab_list.h"
#include "game/oam_cells.h"

void TabList_CountMedals(u16* earnedOut, u16* totalOut);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void TabList_DrawPage(void);
void TabList_DrawPageNumber(void);
void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Gfx_RequestLayerCopy(int layer);
int MainLoop_GetFrameCount(void);
void Palette_Load(const void* src, u16 offset, u32 size);

/* TabListView.unk_1000. Needed to match: the original adds the offset to
 * gMenuState's address at run time. */
#define MEDAL_COLOURS \
  ((u16*)((u8*)&gMenuState + (u32) & ((TabListView*)0)->unk_1000))

/* Highest medal rank: all medals earned. */
#define MEDAL_RANK_MAX 5

/* Per-frame drawing of the tab list screen. On the screen of data_027c0288.unk_00:
 * the four tab sprites, the current one in another palette, then
 * TabList_DrawPage and the page number. On the other screen: earned and total
 * medals, one sprite for the medal rank and one per rank step, and colour
 * 30 of palettes 19-23 black except the rank's own, which cycles through
 * unk_1000 every 4 frames. */
void TabList_DrawFrame(void) {
  u16 earned;
  u16 total;
  int level; /* medal rank: 1-4 at 10, 50, 100, 150 medals, 5 with all of them */
  Unk_027c0280* saved;
  int i;

  TabList_CountMedals(&earned, &total);
  if (earned == total) {
    level = MEDAL_RANK_MAX;
  } else if (earned >= 150) {
    level = 4;
  } else if (earned >= 100) {
    level = 3;
  } else if (earned >= 50) {
    level = 2;
  } else if (earned >= 10) {
    level = 1;
  } else {
    level = 0;
  }

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  for (i = 0; i < TAB_LIST_TAB_COUNT; i++) {
    if (i == gMenuState.tabList.tab) {
      Sprite_QueueAt(0, (i * 64) & 0x1ff, 0, &data_027c0008, i * 32, 0);
    } else {
      Sprite_QueueAt(0, (i * 64) & 0x1ff, 0, &data_027c0008, (i * 32) | 0x1000,
                     0);
    }
  }
  TabList_DrawPage();
  TabList_DrawPageNumber();
  Screen_SetCurrentGfx(saved);

  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Number_Draw(26, 11, data_027c0280->layer0Map, earned, 0, 0);
  Number_Draw(30, 11, data_027c0280->layer0Map, total, 0, 0);
  Gfx_RequestLayerCopy(0);
  Sprite_QueueAt(0, 192, 72, &data_027c0040, 0x2230, 0);
  if (level != 0) {
    Sprite_QueueAt(0, 64, 72, &data_027c0098,
                   ((level - 1) * 64 + 240) | ((level + 7) << 12), 0);
  }
  for (i = 0; i < level; i++) {
    Sprite_QueueAt(0, (8 + i * 48) & 0x1ff, 104, &data_027c0184,
                   (i * 48) | ((i + 3) << 12), 0);
  }
  /* Needed to match: earned is reused as a black colour (a separate local
     * gets its own stack slot). */
  earned = 0;
  for (i = 0; i < MEDAL_RANK_MAX; i++) {
    if (i == level - 1) {
      Palette_Load(&MEDAL_COLOURS[(MainLoop_GetFrameCount() >> 2) & 15],
                   (i + 19) * 32 + 30, 2);
    } else {
      Palette_Load(&earned, (i + 19) * 32 + 30, 2);
    }
  }
  Screen_SetCurrentGfx(saved);
}
