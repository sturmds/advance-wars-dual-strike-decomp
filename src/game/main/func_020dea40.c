#include "types.h"

void ActingUnit_CommitMove(int arg);
void func_020dc910(int arg, int flag);

void func_020dea40(int arg)
{
    ActingUnit_CommitMove(arg);
    func_020dc910(arg, 1);
}
