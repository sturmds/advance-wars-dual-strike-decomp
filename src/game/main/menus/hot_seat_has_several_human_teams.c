#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern u32 gLinkPlay;
extern u8 gSlotControllers
    []; /* per player: PLAYER_CONTROLLER_*, filled by Players_RecordControllers */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define CONTROLLER(player) (gSlotControllers[player])

void Players_RecordControllers(void);
BOOL Player_IsInGame(u8 player);

/* TRUE when human players of two different teams share the DS: not in link
 * play (gLinkPlay), the map has fog or rain, the game is Versus, and two
 * players in the game with a human controller are on different teams. */
BOOL HotSeat_HasSeveralHumanTeams(void) {
  int player;
  u8 firstTeam;
  int humans;

  Players_RecordControllers();
  if (gLinkPlay != 0 ||
      (MAP_STATE->unk_1c4->fog == 0 &&
       MAP_STATE->unk_1c4->weather != WEATHER_RAIN) ||
      MAP_STATE->gameMode != GAME_MODE_VERSUS) {
    return FALSE;
  }
  firstTeam = 0;
  humans = 0;
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (humans == 0) {
      if (CONTROLLER(player) == PLAYER_CONTROLLER_HUMAN) {
        humans++;
        firstTeam = MAP_STATE->players[player].team;
      }
    } else if (CONTROLLER(player) == PLAYER_CONTROLLER_HUMAN &&
               firstTeam != MAP_STATE->players[player].team) {
      return TRUE;
    }
  }
  return FALSE;
}
