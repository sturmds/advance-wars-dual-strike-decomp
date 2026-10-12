#include "types.h"

void ActingUnit_CommitMove(void);
void ActingUnit_AttackObject(int x, int y, int flag);

void ActingUnit_MoveAndAttackObject(int x, int y) {
  ActingUnit_CommitMove();
  ActingUnit_AttackObject(x, y, 0);
}
