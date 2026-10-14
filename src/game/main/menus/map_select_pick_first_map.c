#include "types.h"
#include "game/map_select.h"
#include "game/screen_other_map.h"

void Sound_PlaySe(int id);
void MapPreview_Draw(void* dst, int mapId);
int Map_GetPlayerCount(int mapId);
void MapSelect_BuildLists(
    MapList* lists,
    int playerCount); /* guess: rebuilds the map lists, 0: all maps */
void MapSelect_SelectFirstMap(void);
void Window_Draw(int x, int y, int width, int height, u16* map, int palette);
void Gfx_RequestLayerCopy(int layer);

extern MenuStateFrom400 data_0229ef30;

/* First pick of a two-map battle (MapSelect_PickMap): the selected map
 * becomes the first map (firstMapId). Draws its preview into OBJ VRAM, rebuilds
 * the lists with the first map's player count so the second map can be
 * picked from them, keeps the first map's numbers (unk_4ce) in unk_4d4, and
 * draws a window on the other screen. */
void MapSelect_PickFirstMap(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;

  Sound_PlaySe(0x10006);
  MapPreview_Draw(data_027c0280->objVram + 0xc000, SELECTED_MAP_ID());
  data_0229ef30.firstMapId = SELECTED_MAP_ID();
  MapSelect_BuildLists(MENU->setup.lists,
                       Map_GetPlayerCount(data_0229ef30.firstMapId));
  MapSelect_SelectFirstMap();
  for (i = 0; i < (int)ARRAY_COUNT(MENU->setup.unk_4d4); i++) {
    MENU->setup.unk_4d4[i] = MENU->setup.unk_4ce[i];
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Window_Draw(-1, 7, 10, 9, data_027c0280->layer0Map, 0);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
