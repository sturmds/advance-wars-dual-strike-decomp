#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void LinkPlayer_MarkDefeated(int player, int code);
void Game_EnterState20(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Ends the battle with the team of loser defeated: every player in the game
 * on that team (loser included) is marked defeated with code, then state 20
 * is entered. */
void LinkPlayers_DefeatTeam(int loser, int code) {
  int i;

  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (STATE->players[loser].team == STATE->players[i].team &&
        Player_IsInGame(i)) {
      LinkPlayer_MarkDefeated(i, code);
    }
  }
  Game_EnterState20();
}
