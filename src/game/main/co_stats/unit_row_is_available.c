#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/unlocks.h"
#include "game/player.h"
#include "game/map_header.h"

BOOL Campaign_IsHardMode(void);

extern u8 gUnitRowTypes
    []; /* unit type of each row of the list PlayerInfo_DrawUnitStats draws; UNIT_NONE: empty row */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* playerArmy of the shown player. Needed to match: the compiler adds the
 * player index to gMenuState's address before the field offset. */
#define SHOWN_PLAYER_ARMY                                \
  (((u8*)&gMenuState + (gMenuState.unk_0206d67c.player - \
                        1))[(u32) & ((MenuState*)0)->unk_0206d67c.playerArmy])

/* Whether the unit in a row of PlayerInfo_DrawUnitStats's unit list is shown yet.
 * Outside the campaign only Oozium waits, for game flag 43. In a
 * normal-mode campaign, the newer units appear by campaign map id when the
 * shown player is Black Hole; for other players Piperunner, Black Bomb and
 * Black Boat wait for game flags 99-101 and Oozium never shows. Campaign
 * maps 25-27 hide Megatank, Stealth and Carrier. */
BOOL UnitRow_IsAvailable(int row) {
  u8 army;

  if (gUnitRowTypes[row] == UNIT_NONE) {
    return FALSE;
  }
  if (MAP_STATE->gameMode != GAME_MODE_CAMPAIGN) {
    if (gUnitRowTypes[row] == UNIT_OOZIUM && !GameFlag_IsSet(43)) {
      return FALSE;
    }
    return TRUE;
  }
  if (Campaign_IsHardMode() == TRUE) {
    return TRUE;
  }
  army = SHOWN_PLAYER_ARMY;
  if (army == ARMY_BLACK_HOLE) {
    switch (gUnitRowTypes[row]) {
      case UNIT_MEGATANK:
        if (MAP_STATE->unk_1c4->mapId <= MAP_VERDANT_HILLS) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 25) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 26) {
          return FALSE;
        }
        break;
      case UNIT_STEALTH:
      case UNIT_CARRIER:
        if (MAP_STATE->unk_1c4->mapId <= MAP_CAMPAIGN_FIRST + 13) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 25) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 26) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 27) {
          return FALSE;
        }
        break;
      case UNIT_PIPERUNNER:
      case UNIT_BLACK_BOMB:
      case UNIT_BLACK_BOAT:
        if (MAP_STATE->unk_1c4->mapId <= MAP_CAMPAIGN_FIRST + 8) {
          return FALSE;
        }
        break;
      case UNIT_OOZIUM:
        return MAP_STATE->unk_1c4->mapId >= MAP_MUCK_AMOK;
    }
  } else {
    switch (gUnitRowTypes[row]) {
      case UNIT_MEGATANK:
        /* army 3: probably Green Earth */
        if (MAP_STATE->unk_1c4->mapId == MAP_VERDANT_HILLS && army == 3) {
          return TRUE;
        }
        if (MAP_STATE->unk_1c4->mapId <= MAP_VERDANT_HILLS) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 25) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 26) {
          return FALSE;
        }
        break;
      case UNIT_STEALTH:
      case UNIT_CARRIER:
        if (MAP_STATE->unk_1c4->mapId <= MAP_CAMPAIGN_FIRST + 13) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 25) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 26) {
          return FALSE;
        }
        if (MAP_STATE->unk_1c4->mapId == MAP_CAMPAIGN_FIRST + 27) {
          return FALSE;
        }
        break;
      case UNIT_BLACK_BOAT:
        if (!GameFlag_IsSet(101)) {
          return FALSE;
        }
        break;
      case UNIT_BLACK_BOMB:
        if (!GameFlag_IsSet(100)) {
          return FALSE;
        }
        break;
      case UNIT_PIPERUNNER:
        if (!GameFlag_IsSet(99)) {
          return FALSE;
        }
        break;
      case UNIT_OOZIUM:
        return FALSE;
    }
  }
  return TRUE;
}
