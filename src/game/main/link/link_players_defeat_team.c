#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);
void LinkPlayer_MarkDefeated(int player, int code);
void Game_EnterState20(void);

/* Ends the battle with the team of loser defeated: every player in the game
 * on that team (loser included) is marked defeated with code, then state 20
 * is entered. */
void LinkPlayers_DefeatTeam(int loser, int code) {
  int i;

  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (MAP_STATE->players[loser].team == MAP_STATE->players[i].team &&
        Player_IsInGame(i)) {
      LinkPlayer_MarkDefeated(i, code);
    }
  }
  Game_EnterState20();
}
