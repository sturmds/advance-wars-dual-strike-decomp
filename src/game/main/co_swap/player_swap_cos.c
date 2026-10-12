#include "types.h"
#include "game/Unk_02080440.h"
#include "game/battle_save.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern BattleSaveSkillSet data_022a72c8[5][2];

void MovementCost(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Swaps a player's front and back CO and the skills each lends to the
 * partner, then redoes the movement cost charts (they follow the front CO). */
void Player_SwapCos(int player) {
  PlayerCo front;
  BattleSaveSkillSet lent;

  front = STATE->players[player].front;
  STATE->players[player].front = STATE->players[player].back;
  STATE->players[player].back = front;
  lent = data_022a72c8[player][0];
  data_022a72c8[player][0] = data_022a72c8[player][1];
  data_022a72c8[player][1] = lent;
  MovementCost();
}
