#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
void LinkPlayer_MarkDefeated(int player, int code);
void Game_EnterState20(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Ends the battle with the team of winner as the victor: every player in
 * the game on another team is marked defeated with code, then state 20 is
 * entered. */
void LinkPlayers_DefeatOtherTeams(int winner, int code) {
  int i;

  for (i = 1; i < ARMY_SLOT_COUNT; i++) {
    if (Player_IsInGame(i) &&
        STATE->players[winner].team != STATE->players[i].team) {
      LinkPlayer_MarkDefeated(i, code);
    }
  }
  Game_EnterState20();
}
