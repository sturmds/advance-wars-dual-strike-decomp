#include "types.h"
#include "game/Unk_02080440.h"

/* Save record id of the battle save for a game mode: campaign games use 2 to
 * 4 by campaign slot, the war room 5, versus 6 and survival 1. Other modes
 * have none (0). */
int BattleSave_GetRecordId(int gameMode, int campaignSlot) {
  switch (gameMode) {
    case GAME_MODE_CAMPAIGN:
      switch (campaignSlot) {
        case 0: /* needed to match: default listed first, before cases 1 and 2 */
        default:
          return 2;
        case 1:
          return 3;
        case 2:
          return 4;
      }
    case GAME_MODE_WAR_ROOM:
      return 5;
    case GAME_MODE_VERSUS:
      return 6;
    case GAME_MODE_SURVIVAL:
      return 1;
    default:
      return 0;
  }
}
