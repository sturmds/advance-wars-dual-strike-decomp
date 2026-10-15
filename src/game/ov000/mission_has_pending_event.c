#include "types.h"
#include "game/screen_other_map.h"
#include "game/unlocks.h"
#include "game/map_object.h"

extern Unk_02080440_48 data_02183b08;
MapObject* Map_FindObjectOfKindCovering(int x, int y, int kind);

/* Looks up the map region at (x, y) with the given kind (Map_FindObjectOfKindCovering) on the other screen. */
static inline MapObject* OtherScreen_FindRegion(int x, int y, int kind) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  MapObject* result;

  Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
  result = Map_FindObjectOfKindCovering(x, y, kind);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return result;
}

int Mission_HasPendingEvent(int event) {
  int result = 0;

  if (MAP_STATE->gameMode != GAME_MODE_CAMPAIGN) {
    return 0;
  }
  if (MAP_STATE != &data_02183b08) {
    return 0;
  }
  switch (MAP_STATE->unk_1c4->mapId) {
    case 238:
      if (!GameFlag_IsSet(4) && event == 4) {
        result = 1;
      }
      break;
    case 242:
      if (!GameFlag_IsSet(1) && event == 10) {
        result = 1;
      }
      break;
    case 248:
      if (event == 11) {
        if (OtherScreen_FindRegion(1, 1, 9) != NULL) {
          result = 1;
        }
      } else if (event == 12) {
        if (OtherScreen_FindRegion(8, 1, 9) != NULL) {
          result = 1;
        }
      } else if (event == 13) {
        if (OtherScreen_FindRegion(14, 1, 9) != NULL) {
          result = 1;
        }
      }
      break;
  }
  return result;
}
