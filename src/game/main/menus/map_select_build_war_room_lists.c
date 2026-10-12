#include "types.h"
#include "game/menu_state.h"

BOOL MapSelect_FillWarRoomList(MapList* list, int kind);

/* Builds the four War Room lists (kinds 9-12), skipping empty ones. */
void MapSelect_BuildWarRoomLists(MapListSet* set) {
  int kind;

  set->count = 0;
  for (kind = 9; kind < 13; kind++) {
    if (MapSelect_FillWarRoomList(&set->lists[set->count], kind) == 1) {
      set->count++;
    }
  }
}
