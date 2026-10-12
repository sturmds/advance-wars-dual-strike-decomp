#include "types.h"

int MoveSearch_FindTile(int x, int y, int unitType, int movement);
int Ai_CanDamageEnemyAtTile(int a, int b, int c);

/* Callback the path search at MoveSearch_FindTile calls for each tile. */
extern int (*data_0229e174)(int a, int b, int c);

/* Runs the path search with the Ai_CanDamageEnemyAtTile callback installed. */
int func_020a50d4(int x, int y, int unitType, int movement) {
  data_0229e174 = Ai_CanDamageEnemyAtTile;
  return MoveSearch_FindTile(x, y, unitType, movement);
}
