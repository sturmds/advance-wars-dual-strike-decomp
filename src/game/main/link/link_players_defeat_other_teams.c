#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);
void LinkPlayer_MarkDefeated(int player, int code);
void Game_EnterState20(void);

/* Ends the battle with the team of winner as the victor: every player in
 * the game on another team is marked defeated with code, then state 20 is
 * entered. */
void LinkPlayers_DefeatOtherTeams(int winner, int code) {
  int i;

  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (Player_IsInGame(i) &&
        MAP_STATE->players[winner].team != MAP_STATE->players[i].team) {
      LinkPlayer_MarkDefeated(i, code);
    }
  }
  Game_EnterState20();
}
