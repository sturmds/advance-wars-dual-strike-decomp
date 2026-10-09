#include "types.h"

void ActingUnit_CommitMove(void);
int ActingUnit_StartAttack(int, int);

int func_020dea20(int a0)
{
    ActingUnit_CommitMove();
    return ActingUnit_StartAttack(a0, 0);
}
