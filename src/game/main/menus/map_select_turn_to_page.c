#include "types.h"
#include "game/map_select.h"

s32 HwDiv32(s32 numerator, s32 denominator);
void Sound_PlaySe(int id);
void MapSelect_DrawMapNames(int list, int page);
void MapSelect_AddMapButtons(void);
void MapSelect_LoadEnemyCoPictures(void);
void MapSelect_LoadWarRoomRecords(void);
void MapSelect_ShowSelectedMap(void);

/* Turns the current list to page, if the list has that many pages. The
 * first change plays a sound (shared with the list changes). Then redraws
 * the map names and buttons of the page and what depends on the selected
 * map. */
void MapSelect_TurnToPage(u32 page) {
  if (page >= HwDiv32(LIST(TAB()).mapCount - 1, ROWS * COLS) + 1) {
    return;
  }
  PAGE() = page;
  if (MENU->setup.listChanged == 0) {
    Sound_PlaySe(0x10002);
    MENU->setup.listChanged = 1;
  }
  MapSelect_DrawMapNames(LIST_OF_TAB(TAB()), PAGE());
  MapSelect_AddMapButtons();
  MapSelect_LoadEnemyCoPictures();
  MapSelect_LoadWarRoomRecords();
  MapSelect_ShowSelectedMap();
}
