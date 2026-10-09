#include "types.h"

void ActingUnit_CommitMove(void);
void ActingUnit_StartAttack(int arg, int flag);

void func_020dea40(int arg)
{
    ActingUnit_CommitMove();
    ActingUnit_StartAttack(arg, 1);
}
