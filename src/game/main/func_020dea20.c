#include "types.h"

void ActingUnit_CommitMove(void);
void ActingUnit_StartAttack(int defenderSlot, int flag);

void func_020dea20(int a0) {
  ActingUnit_CommitMove();
  ActingUnit_StartAttack(a0, 0);
}
