#include "types.h"
#include "game/map_select.h"

void Sound_PlaySe(int id);
void MapSelect_LoadEnemyCoPictures(void);
void MapSelect_LoadWarRoomRecords(void);
void MapSelect_ShowSelectedMap(void);

/* Selects the map at position pos of the shown page of the current list,
 * if the list has a map there. The first change plays a sound (shared with
 * the list changes). Then redraws what depends on the selected map. */
void MapSelect_SelectPagePosition(int pos) {
  if (pos + PAGE() * (ROWS * COLS) >= LIST(TAB()).mapCount) {
    return;
  }
  if (MENU->setup.listChanged == 0) {
    Sound_PlaySe(0x10002);
    MENU->setup.listChanged = 1;
  }
  TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()] = pos;
  MapSelect_LoadEnemyCoPictures();
  MapSelect_LoadWarRoomRecords();
  MapSelect_ShowSelectedMap();
}
