#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Player_RefreshUnits(void);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define EXTRA (data_027c027c->unk_5c[data_027c0284])

/* Display step of action 13. Guess: a new turn starts. Refreshes the units,
 * restarts the AI turn and copies the next weather and the per-player weather days from
 * the action. */
void AiShowAction_StartTurn(void) {
  int i;

  GAME->unk_18 = 1;
  Player_RefreshUnits();
  gAiTurnState->state = AI_STATE_START;
  GAME->unk_1c4->nextWeather = gUnitAction.target;
  EXTRA->sandChance = gUnitAction.unk_07;
  EXTRA->rainChance = gUnitAction.packed[0];
  EXTRA->snowChance = gUnitAction.packed[1];
  for (i = 0; i < PLAYER_SLOT_LAST; i++) {
    EXTRA->weatherDays[i + 1] = gUnitAction.packed[i + 2];
  }
}
