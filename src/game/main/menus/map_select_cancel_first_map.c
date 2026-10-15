#include "types.h"
#include "game/map_select.h"
#include "game/screen_other_map.h"

void Sound_PlaySe(int id);
void MapSelect_BuildLists(
    MapList* lists,
    int playerCount); /* guess: rebuilds the map lists, 0: all maps */
void MapSelect_DrawMapNames(int list, int page);
void MapSelect_AddMapButtons(void);
void Bg_ClearRect(u16* map, int x, int y, int w, int h);
void Gfx_RequestLayerCopy(int layer);

/* Undoes the first pick of a two-map battle (MapSelect_PickFirstMap): clears
 * the first map, rebuilds the lists with all maps, redraws the page and
 * clears the window on the other screen. */
void MapSelect_CancelFirstMap(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  Sound_PlaySe(0x10009);
  gMenuState.setup.firstMapId = 0;
  MapSelect_BuildLists(MENU->setup.lists, 0);
  MapSelect_DrawMapNames(LIST_OF_TAB(TAB()), PAGE());
  MapSelect_AddMapButtons();
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Bg_ClearRect(data_027c0280->layer0Map, 0, 7, 9, 9);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
