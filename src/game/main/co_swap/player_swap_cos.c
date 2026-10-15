#include "types.h"
#include "game/Unk_02080440.h"
#include "game/battle_save.h"

extern BattleSaveSkillSet data_022a72c8[][2];

void MovementCost(void);

/* Swaps a player's front and back CO and the skills each lends to the
 * partner, then redoes the movement cost charts (they follow the front CO). */
void Player_SwapCos(int player) {
  PlayerCo front;
  BattleSaveSkillSet lent;

  front = MAP_STATE->players[player].front;
  MAP_STATE->players[player].front = MAP_STATE->players[player].back;
  MAP_STATE->players[player].back = front;
  lent = data_022a72c8[player][0];
  data_022a72c8[player][0] = data_022a72c8[player][1];
  data_022a72c8[player][1] = lent;
  MovementCost();
}
