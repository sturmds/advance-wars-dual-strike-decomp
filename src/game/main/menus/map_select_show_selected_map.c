#include "types.h"
#include "game/map_header.h"
#include "game/map_select.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"

void Bg_ClearRect(u16* map, int x, int y, int w, int h);
void Gfx_RequestLayerCopy(int bg);
const void* Map_GetName(u32 mapId);
u8 Map_IsFlagSet(u32 mapId);
void Screen_SetCurrentGfx(void* state);
void Window_Draw(int x, int y, int width, int height, u16* map, int palette);
void Mem_Zero32(u32 value, void* dst, u32 size);
void MapPreview_CopyCounts(s8* dst);
void MapPreview_Draw(void* dst, int mapId);

extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u8 data_02167c90[];

/* Map that goes with the selected one in a two-map battle; 0 if none. */
#define SECOND_MAP_ID() (Ov000_gMapHeaders[SELECTED_MAP_ID()].variantMapIds[0])

/* Needed to match: through an inline function the compiler loads the map
 * before the MapSelect_GetTab calls, as the original does. */
static inline u16* Gfx_GetLayer1Map(void) { return data_027c0280->layer1Map; }

/* Shows the selected map of the map select menu. On the screen of
 * data_027c0288.unk_04: the map preview and name, or a cleared area and the
 * data_02167c90 text while the map is locked (Map_IsFlagSet). In gameMode 1,
 * a map with a second map also gets a preview on the other screen and a
 * window. Then fills the number grids that MapSelect_Draw shows:
 * MapPreview_CopyCounts copies six counts that the last MapPreview_Draw call left. */
void MapSelect_ShowSelectedMap(void) {
  Unk_027c0280* savedState;
  Unk_027c0280* savedState2;
  Unk_027c0280* savedState3;
  int i;

  savedState = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  if (Map_IsFlagSet(SELECTED_MAP_ID())) {
    if (STATE->gameMode == GAME_MODE_WAR_ROOM && SECOND_MAP_ID() != 0) {
      MapPreview_Draw(data_027c0280->objVram + 0x8000, SECOND_MAP_ID());
      MapPreview_CopyCounts(MENU->setup.unk_4d4);
    } else {
      MapPreview_Draw(data_027c0280->objVram + 0x8000, SELECTED_MAP_ID());
    }
    Bg_ClearRect(data_027c0280->layer1Map, 12, 21, 8, 2);
    Text_Print(12, 21, Gfx_GetLayer1Map(), Map_GetName(SELECTED_MAP_ID()), 0,
               0x100);
  } else {
    Mem_Zero32(0, data_027c0280->objVram + 0x8000, 0x4000);
    Bg_ClearRect(data_027c0280->layer1Map, 12, 21, 8, 2);
    Text_Print(12, 21, data_027c0280->layer1Map, data_02167c90, 0, 0x100);
  }
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentGfx(savedState);

  savedState2 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
    if (SECOND_MAP_ID() != 0) {
      if (Map_IsFlagSet(SELECTED_MAP_ID())) {
        MapPreview_Draw(data_027c0280->objVram + 0xc000, SELECTED_MAP_ID());
      } else {
        Mem_Zero32(0, data_027c0280->objVram + 0xc000, 0x4000);
      }
    }
    savedState3 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    if (SECOND_MAP_ID() != 0) {
      Window_Draw(-1, 7, 10, 9, data_027c0280->layer0Map, 0);
    } else {
      Bg_ClearRect(data_027c0280->layer0Map, 0, 7, 9, 9);
    }
    Gfx_RequestLayerCopy(0);
    Screen_SetCurrentGfx(savedState3);
  }
  Screen_SetCurrentGfx(savedState2);

  /* Ids from 0x101 show -1 in every count. */
  if (SELECTED_MAP_ID() >= 0x101) {
    for (i = 0; i < (int)ARRAY_COUNT(MENU->setup.unk_4ce); i++) {
      MENU->setup.unk_4ce[i] = -1;
    }
    return;
  }
  if (!Map_IsFlagSet(SELECTED_MAP_ID())) {
    for (i = 0; i < (int)ARRAY_COUNT(MENU->setup.unk_4ce); i++) {
      MENU->setup.unk_4ce[i] = 0;
      if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
        MENU->setup.unk_4d4[i] = 0;
      }
    }
    return;
  }
  if (TAB() == 2) {
    MapPreview_CopyCounts(MENU->setup.unk_4d4);
  } else {
    MapPreview_CopyCounts(MENU->setup.unk_4ce);
  }
}
