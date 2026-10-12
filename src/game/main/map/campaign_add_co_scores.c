#include "types.h"
#include "game/Unk_02080440.h"
/* Needed to match: data_022a7a9c is read with its rows (as in
 * battle_results_fill.c), so hide the header's one-entry declaration. */
#define data_022a7a9c data_022a7a9c_entry
#include "game/Unk_022a7a9c.h"
#undef data_022a7a9c
#include "game/Unk_02290118.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern BattleResults data_022a7a9c;

BOOL Player1Team_HasPlayerInGame(void);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* After a campaign battle that player 1's team has not lost, adds 1 to the
 * saved score of each CO a human player used (the world map shows the top
 * three). */
void Campaign_AddCoScores(void) {
  int player;
  int slot;
  int co;

  if (GAME->gameMode != GAME_MODE_CAMPAIGN) {
    return;
  }
  if (!Player1Team_HasPlayerInGame()) {
    return;
  }
  for (player = 1; player < 5; player++) {
    if (GAME->players[player].controller == PLAYER_CONTROLLER_HUMAN) {
      for (slot = 0; slot < 2; slot++) {
        co = (&data_022a7a9c.rows[player].unk_00)[slot] & PLAYER_CO_ID_MASK;
        if (co != 0) {
          data_02290118.coScore[co]++;
        }
      }
    }
  }
}
