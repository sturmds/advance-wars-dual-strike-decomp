#include "types.h"
#include "game/Unk_02080440.h"

extern u32 data_021663a0[];

void EventScript_StartText(u32 script, int arg1, int arg2);
void LinkPlayerDefeatTask_Create(u16 player, u16 code);
void LinkPlayer_MarkDefeated(int player, int code);
void Player_RefreshUnits(void);
void Game_EnterState18(void);

/* Takes a player out of a link battle for the reason in code (the values
 * func_020cef2c passes: 1, 2, 8, 16). LinkPlayer_MarkDefeated marks the player defeated;
 * the name is a guess from that. */
void LinkPlayer_Defeat(int player, int code) {
  EventScript_StartText(
      data_021663a0[MAP_STATE->players[player].armyColour - 1], 21, 0);
  LinkPlayerDefeatTask_Create(player, code);
  LinkPlayer_MarkDefeated(player, code);
  if (player == MAP_STATE->currentPlayer) {
    MAP_STATE->unk_18 = 1;
    Player_RefreshUnits();
  }
  if (code == 2) {
    Game_EnterState18();
  }
}
