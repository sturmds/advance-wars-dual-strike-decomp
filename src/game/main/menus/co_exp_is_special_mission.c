#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/map_header.h"
#include "game/screen_other_map.h"

/* TRUE in a campaign battle on mission 8, 18 or 24 (map ids 0xe0 + n), read
 * from the map id of the first screen's state. */
BOOL CoExp_IsSpecialMission(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16 mapId;

  if (data_027c027c->unk_48[data_027c0284]->gameMode != GAME_MODE_CAMPAIGN) {
    return FALSE;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  mapId = data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (mapId == MAP_CAMPAIGN_FIRST + 8 || mapId == MAP_CAMPAIGN_FIRST + 18 ||
      mapId == MAP_CAMPAIGN_FIRST + 24) {
    return TRUE;
  }
  return FALSE;
}
