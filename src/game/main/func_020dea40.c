#include "types.h"

void ActingUnit_CommitMove(int arg);
void ActingUnit_StartAttack(int arg, int flag);

void func_020dea40(int arg)
{
    ActingUnit_CommitMove(arg);
    ActingUnit_StartAttack(arg, 1);
}
