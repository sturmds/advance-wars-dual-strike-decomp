#include "types.h"
#include "game/map_header.h"
#include "game/map_select.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator); /* remainder, hardware divider */
int MainLoop_GetFrameCount(void);
void Co_QueueSmallPictureSpriteAtTile(int x, int y, int attr, int tile, int a4);
void Palette_Load(const void* src, int slideOffset, int size);
void Screen_SetCurrentGfx(void* state);
void Sprite_Queue(int a0, int x, int y, void* a3, int attr, int tile);
void Sprite_QueueAt(int a0, int x, int y, void* a3, int attr, int tile);
int Math_EaseInOut(int from, int to, int step, int steps);
void Sprite_QueuePicture192x160(s16 x, int y, int a2, int a3, int a4);
void MapSelect_EnableSubSlideButtons(void);
void MapSelect_OnSlideDone(void);
void MapSelect_DrawStatsPanel(int x, int y, int a2);
void MapSelect_DrawPageIndicator(int x, int y, int list);
void MapSelect_LoadWarRoomRecords(void);
void MapSelect_ShowSelectedMap(void);
void MapSelect_DrawSubStrip(int x, int y, int a2);
void MapSelect_DrawTwoMapsTitle(int x, int y);
void MapSelect_DrawListThumbnail(int x, int y, int list);
void MapSelect_DrawMapNames(int list, int a1);
int Link_GetOwnPlayer(void);
int Link_IsActive(void);
int Map_GetPlayerCount(int mapId);

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_027c0000;
extern u8 data_027c0028;
extern u8 data_027c0040;
extern u8 data_027c00b8;
extern u8 data_027c0108;

/* Writes value to the X scroll of the BG used by layer bg, then copies
 * it to the BG of layer copyBg. */
#define SET_SCROLL(bg, copyBg, value)                              \
  {                                                                \
    u8 from = data_027c0280->bg;                                   \
    u8 to = data_027c0280->copyBg;                                 \
    data_027c0280->bgScrollX[from] = (value);                      \
    data_027c0280->bgScrollX[to] = data_027c0280->bgScrollX[from]; \
  }
/* Six values in a 3 by 2 grid: an icon (grey when the value is 0), then
 * the value in up to two digits under it. */
#define DRAW_NUMBER_GRID(values, y)                                            \
  for (i = 0; i < 6; i++) {                                                    \
    Sprite_QueueAt(1, (HwMod32(i, 3) * 21 + 4) & 0x1ff,                        \
                   (HwDiv32(i, 3) * 28 + (y)) & 0xff, &data_027c0000,          \
                   ((values)[i] != 0 ? 10 : 11) << 12, 124 + i * 8);           \
    value = (values)[i];                                                       \
    if (value > 0) {                                                           \
      int ones = HwMod32(value, 10);                                           \
      Sprite_QueueAt(0, (HwMod32(i, 3) * 21 + 12) & 0x1ff,                     \
                     (HwDiv32(i, 3) * 28 + (y) + 24) & 0xff, &data_027c0028,   \
                     0x3000, ones * 2 + 104);                                  \
      value = HwDiv32(value, 10);                                              \
      if (value != 0) {                                                        \
        Sprite_QueueAt(0, (HwMod32(i, 3) * 21 + 4) & 0x1ff,                    \
                       (HwDiv32(i, 3) * 28 + (y) + 24) & 0xff, &data_027c0028, \
                       0x3000, value * 2 + 104);                               \
      }                                                                        \
    }                                                                          \
  }

/* Draws one frame of the map select menu. data_027c0288 holds a draw state
 * per screen; Screen_SetCurrentGfx switches between them. While slideFrames runs
 * (15 down to 1) the lists slide sideways, eased from 256 pixels to 0; the
 * second slide does the same for the strip at TABLES.subStrip. Then: the map
 * list thumbnails, a marker per player for the selected map's player count,
 * and the number grids. In gameMode 1 two palette colours cycle every 4
 * frames. */
void MapSelect_Draw(void) {
  int slideOffset;
  int subSlideOffset;
  int thumbOffset;
  Unk_027c0280* savedState;
  int count;
  Unk_027c0280* savedState2;
  int i;
  int j;
  int value;

  if (MENU->setup.slideFrames != 0) {
    if (MENU->setup.slideFrames == 15) {
      MapSelect_DrawMapNames(LIST_OF_TAB(TAB()), PAGE());
      MapSelect_LoadWarRoomRecords();
      MapSelect_ShowSelectedMap();
    }
    slideOffset = Math_EaseInOut(256, 0, 17 - MENU->setup.slideFrames, 16);
    if (MENU->setup.slideBack != 0) {
      SET_SCROLL(layer1Bg, layer0Bg, -slideOffset);
      savedState = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      SET_SCROLL(layer2Bg, layer1Bg, -slideOffset);
      Screen_SetCurrentGfx(savedState);
    } else {
      SET_SCROLL(layer1Bg, layer0Bg, slideOffset);
      savedState = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      SET_SCROLL(layer2Bg, layer1Bg, slideOffset);
      Screen_SetCurrentGfx(savedState);
    }
    MENU->setup.slideFrames--;
    if (MENU->setup.slideFrames == 0) {
      MapSelect_OnSlideDone();
    }
  }
  if (MENU->setup.subSlideFrames != 0) {
    subSlideOffset =
        Math_EaseInOut(256, 0, 17 - MENU->setup.subSlideFrames, 16);
    MENU->setup.subSlideFrames--;
    if (MENU->setup.subSlideFrames == 0) {
      MapSelect_EnableSubSlideButtons();
    }
  }

  savedState = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  if (MENU->setup.slideFrames != 0) {
    if (MENU->setup.slideBack != 0) {
      thumbOffset = slideOffset >> 1;
      MapSelect_DrawListThumbnail(thumbOffset - 128, 8, LIST_OF_TAB(TAB()) - 2);
      MapSelect_DrawListThumbnail(thumbOffset, 8, LIST_OF_TAB(TAB()) - 1);
      MapSelect_DrawListThumbnail(thumbOffset + 128, 8, LIST_OF_TAB(TAB()));
      MapSelect_DrawListThumbnail(thumbOffset + 256, 8, LIST_OF_TAB(TAB()) + 1);
      if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
        MapSelect_DrawStatsPanel(slideOffset, 72, 0);
        MapSelect_DrawStatsPanel(slideOffset - 256, 72, 1);
        MapSelect_DrawPageIndicator(slideOffset + 200, 64, LIST_OF_TAB(TAB()));
        MapSelect_DrawPageIndicator(slideOffset - 56, 64, MENU->setup.prevList);
      } else if (STATE->twoMaps == 0) {
        MapSelect_DrawPageIndicator(slideOffset + 192, 32, LIST_OF_TAB(TAB()));
        MapSelect_DrawPageIndicator(slideOffset - 64, 32, MENU->setup.prevList);
      } else {
        MapSelect_DrawPageIndicator(slideOffset + 192, 48, LIST_OF_TAB(TAB()));
        MapSelect_DrawPageIndicator(slideOffset - 64, 48, MENU->setup.prevList);
      }
    } else {
      thumbOffset = slideOffset >> 1;
      MapSelect_DrawListThumbnail(-thumbOffset, 8, LIST_OF_TAB(TAB()) - 1);
      MapSelect_DrawListThumbnail(128 - thumbOffset, 8, LIST_OF_TAB(TAB()));
      MapSelect_DrawListThumbnail(256 - thumbOffset, 8, LIST_OF_TAB(TAB()) + 1);
      MapSelect_DrawListThumbnail(384 - thumbOffset, 8, LIST_OF_TAB(TAB()) + 2);
      if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
        MapSelect_DrawStatsPanel(-slideOffset, 72, 0);
        MapSelect_DrawStatsPanel(256 - slideOffset, 72, 1);
        MapSelect_DrawPageIndicator(200 - slideOffset, 64, LIST_OF_TAB(TAB()));
        MapSelect_DrawPageIndicator(456 - slideOffset, 64,
                                    MENU->setup.prevList);
      } else if (STATE->twoMaps == 0) {
        MapSelect_DrawPageIndicator(192 - slideOffset, 32, LIST_OF_TAB(TAB()));
        MapSelect_DrawPageIndicator(448 - slideOffset, 32,
                                    MENU->setup.prevList);
      } else {
        MapSelect_DrawPageIndicator(192 - slideOffset, 48, LIST_OF_TAB(TAB()));
        MapSelect_DrawPageIndicator(448 - slideOffset, 48,
                                    MENU->setup.prevList);
      }
    }
  } else {
    MapSelect_DrawListThumbnail(0, 8, LIST_OF_TAB(TAB()) - 1);
    MapSelect_DrawListThumbnail(128, 8, LIST_OF_TAB(TAB()));
    MapSelect_DrawListThumbnail(256, 8, LIST_OF_TAB(TAB()) + 1);
    if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
      MapSelect_DrawPageIndicator(200, 64, LIST_OF_TAB(TAB()));
      MapSelect_DrawStatsPanel(0, 72, 0);
    } else if (STATE->twoMaps == 0) {
      MapSelect_DrawPageIndicator(192, 32, LIST_OF_TAB(TAB()));
    } else {
      MapSelect_DrawPageIndicator(192, 48, LIST_OF_TAB(TAB()));
    }
  }

  if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
    if (MENU->setup.subSlideFrames != 0) {
      if (MENU->setup.subSlideBack != 0) {
        subSlideOffset >>= 1;
        MapSelect_DrawSubStrip(subSlideOffset - 128, 40, TABLES.subStrip - 2);
        MapSelect_DrawSubStrip(subSlideOffset, 40, TABLES.subStrip - 1);
        MapSelect_DrawSubStrip(subSlideOffset + 128, 40, TABLES.subStrip);
        MapSelect_DrawSubStrip(subSlideOffset + 256, 40, TABLES.subStrip + 1);
      } else {
        subSlideOffset >>= 1;
        MapSelect_DrawSubStrip(-subSlideOffset, 40, TABLES.subStrip - 1);
        MapSelect_DrawSubStrip(128 - subSlideOffset, 40, TABLES.subStrip);
        MapSelect_DrawSubStrip(256 - subSlideOffset, 40, TABLES.subStrip + 1);
        MapSelect_DrawSubStrip(384 - subSlideOffset, 40, TABLES.subStrip + 2);
      }
    } else {
      MapSelect_DrawSubStrip(0, 40, TABLES.subStrip - 1);
      MapSelect_DrawSubStrip(128, 40, TABLES.subStrip);
      MapSelect_DrawSubStrip(256, 40, TABLES.subStrip + 1);
    }
  } else if (STATE->twoMaps == 1) {
    MapSelect_DrawTwoMapsTitle(128, 8);
  }

  if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
    if (MENU->setup.slideFrames != 0) {
      if (MENU->setup.slideBack != 0) {
        if (LIST(TAB()).unk_64 == 12) {
          Sprite_QueuePicture192x160(slideOffset + 128, 120, 0x2800, 0x600, 3);
        } else if (MENU->setup.lists[MENU->setup.prevList].unk_64 == 12) {
          Sprite_QueuePicture192x160(slideOffset - 128, 120, 0x2800, 0x600, 3);
        }
      } else {
        if (LIST(TAB()).unk_64 == 12) {
          Sprite_QueuePicture192x160(128 - slideOffset, 120, 0x2800, 0x600, 3);
        } else if (MENU->setup.lists[MENU->setup.prevList].unk_64 == 12) {
          Sprite_QueuePicture192x160(384 - slideOffset, 120, 0x2800, 0x600, 3);
        }
      }
    } else if (LIST(TAB()).unk_64 == 12) {
      Sprite_QueuePicture192x160(128, 120, 0x2800, 0x600, 3);
    }
  } else if (MENU->setup.firstMapId != 0) {
    Sprite_QueuePicture192x160(128, 112, 0x2800, 0x600, 3);
  }
  Screen_SetCurrentGfx(savedState);

  savedState2 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  if (MENU->setup.slideFrames != 0) {
    if (MENU->setup.slideBack != 0) {
      Sprite_QueuePicture192x160(slideOffset + 128, 96, 0x2c00, 0x400, 3);
      Sprite_QueuePicture192x160(slideOffset - 128, 96, 0x2c00, 0x600, 3);
    } else {
      Sprite_QueuePicture192x160(128 - slideOffset, 96, 0x2c00, 0x400, 3);
      Sprite_QueuePicture192x160(384 - slideOffset, 96, 0x2c00, 0x600, 3);
    }
  } else {
    Sprite_QueuePicture192x160(128, 96, 0x2c00, 0x400, 3);
  }

  if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
    count = Map_GetPlayerCount(SELECTED_MAP_ID());
    for (i = 0; i < count - 1; i++) {
      for (j = 0; j <= 1; j++) {
        int k = j + i * 2;
        Co_QueueSmallPictureSpriteAtTile(((232 - j * 32) & 0x1ff) | 0x1000,
                                         (192 - i * 16) & 0xff, (k + 4) << 12,
                                         k * 12 + 16, (k & 1) + 1);
      }
    }
    Sprite_QueueAt(0, 208, (192 - count * 16) & 0xff, &data_027c00b8, 0x3000,
                   0);
  }

  DRAW_NUMBER_GRID(MENU->setup.unk_4ce, 128);

  /* The second grid shows on tab 2 and for a map with a second map. */
  if (TAB() == 2 ||
      (STATE->gameMode == GAME_MODE_WAR_ROOM &&
       Ov000_gMapHeaders[SELECTED_MAP_ID()].variantMapIds[0] != 0)) {
    DRAW_NUMBER_GRID(MENU->setup.unk_4d4, 56);
  }
  Screen_SetCurrentGfx(savedState2);

  if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
    Palette_Load(&MENU->colours[(MainLoop_GetFrameCount() >> 2) & 15], 0x238,
                 2);
  }
  if (Link_IsActive() == 1 && Link_GetOwnPlayer() != 0) {
    Sprite_Queue(0, 172, 172, &data_027c0040, 0x3000, 0x5c4);
    Sprite_Queue(0, 188, 172, &data_027c0108, 0x3000, 0x5e8);
    Palette_Load(&MENU->setup.colours2[(MainLoop_GetFrameCount() >> 2) & 15],
                 0x274, 2);
  }
}
