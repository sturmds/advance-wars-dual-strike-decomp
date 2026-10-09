#include "types.h"

void ActingUnit_CommitMove(void);
void func_020dd264(int x, int y, int flag); /* hits the map object at (x, y); callee not decompiled yet */

void ActingUnit_MoveAndAttackObject(int x, int y)
{
    ActingUnit_CommitMove();
    func_020dd264(x, y, 0);
}
