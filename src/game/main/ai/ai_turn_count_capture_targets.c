#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

BOOL Player_IsInGame(u8 player);
/* 1 + the properties the player owns (bases, cities, airports, ports, Com Towers). */
int Player_CountPropertiesPlusOne(int player);

/* Totals Player_CountPropertiesPlusOne over the neutral player and every player in the game
 * on another team. The name is a guess from what it counts. */
void AiTurn_CountCaptureTargets(void) {
  int player;

  gAiTurnState->captureTargets = 0;
  gAiTurnState->captureTargets += Player_CountPropertiesPlusOne(0);
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        GAME->players[GAME->currentPlayer].team != GAME->players[player].team) {
      gAiTurnState->captureTargets += Player_CountPropertiesPlusOne(player);
    }
  }
}
