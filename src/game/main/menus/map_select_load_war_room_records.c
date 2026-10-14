#include "types.h"
#include "game/map_select.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"
#include "game/war_room_records.h"

void Co_QueueSmallPicture(int coId, void* dest, int paletteSlot);

extern Unk_027c0280* data_027c0280;

/* Record of the selected map in place 0-2. */
#define RECORD(place)                                                       \
  (gWarRoomRecords[TABLES.subStrip][SELECTED_MAP_ID() - WAR_ROOM_FIRST_MAP] \
                  [place])

/* War Room map select: copies the three records of the selected map into
 * the menu state and loads the CO pictures of each record (front CO, then
 * tag partner) into OBJ VRAM, 12 tiles apart from tile 0x1ca, with
 * palettes 19-24. Does nothing outside gameMode 1. */
void MapSelect_LoadWarRoomRecords(void) {
  int i;

  if (STATE->gameMode != GAME_MODE_WAR_ROOM) {
    return;
  }
  for (i = 0; i < (int)ARRAY_COUNT(MENU->setup.recordDays); i++) {
    MENU->setup.recordDays[i] = RECORD(i).days;
    MENU->setup.recordScores[i] = RECORD(i).score;
    if (MENU->setup.recordDays[i] == 0) {
      MENU->setup.recordCos[i * 2] = 0;
      MENU->setup.recordCos[i * 2 + 1] = 0;
    } else {
      MENU->setup.recordCos[i * 2] = RECORD(i).frontCo;
      MENU->setup.recordCos[i * 2 + 1] = RECORD(i).backCo;
    }
    if (MENU->setup.recordCos[i * 2] != 0) {
      Co_QueueSmallPicture(MENU->setup.recordCos[i * 2],
                           data_027c0280->objVram + (0x1ca + i * 2 * 12) * 32,
                           i * 2 + 19);
    }
    if (MENU->setup.recordCos[i * 2 + 1] != 0) {
      Co_QueueSmallPicture(
          MENU->setup.recordCos[i * 2 + 1],
          data_027c0280->objVram + (0x1ca + (i * 2 + 1) * 12) * 32,
          i * 2 + 1 + 19);
    }
  }
}
