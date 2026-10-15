#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

extern Unk_02080440_48 data_02183b08;
extern Unk_02080440_48 data_02189cd8;

#define OWN_MAP ((&data_027c0288.unk_00)[data_027c0284])
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Whether the current screen's map state should play now. With one map, or
 * when the other map's unk_30 is nonzero (Turn_AdvanceToNextPlayer reads it
 * as the other map being over), only data_02183b08 plays. In a two-map
 * battle with both maps going, the map on the earlier day plays, and the
 * first map on a tie. */
BOOL Map_IsThisMapsTurn(void) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u32 otherUnk30;
  int ownDay;
  int otherDay;
  BOOL ownNotLater;

  if (MAP_STATE->secondMapId != 0) {
    SWITCH_TO(OTHER_MAP);
    otherUnk30 = MAP_STATE->unk_1c4->unk_30;
    SWITCH_BACK();
    if (otherUnk30 == 0) {
      goto compare;
    }
  }
  if (MAP_STATE == &data_02183b08) {
    return TRUE;
  }
  if (MAP_STATE == &data_02189cd8) {
    return FALSE;
  }
compare:
  SWITCH_TO(OWN_MAP);
  ownDay = MAP_STATE->day;
  SWITCH_BACK();
  SWITCH_TO(OTHER_MAP);
  otherDay = MAP_STATE->day;
  SWITCH_BACK();
  ownNotLater = ownDay <= otherDay ? TRUE : FALSE;
  if (MAP_STATE == &data_02183b08) {
    return ownNotLater;
  }
  return ownNotLater == FALSE;
}
