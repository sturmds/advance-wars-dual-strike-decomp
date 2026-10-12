#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern u8 gSlotControllers
    []; /* per player: PLAYER_CONTROLLER_*, filled by Players_RecordControllers */

#define STATE (data_027c027c->unk_48[data_027c0284])
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
      (STATE->unk_1c4->fog == 0 && STATE->unk_1c4->weather != WEATHER_RAIN) ||
      STATE->gameMode != GAME_MODE_VERSUS) {
    return FALSE;
  }
  firstTeam = 0;
  humans = 0;
  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (humans == 0) {
      if (CONTROLLER(player) == PLAYER_CONTROLLER_HUMAN) {
        humans++;
        firstTeam = STATE->players[player].team;
      }
    } else if (CONTROLLER(player) == PLAYER_CONTROLLER_HUMAN &&
               firstTeam != STATE->players[player].team) {
      return TRUE;
    }
  }
  return FALSE;
}
