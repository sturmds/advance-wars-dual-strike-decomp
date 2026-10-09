#include "types.h"

void ActingUnit_CommitMove(void);
void func_020dd264(int x, int y, int flag);

void func_020de9f0(int x, int y)
{
    ActingUnit_CommitMove();
    func_020dd264(x, y, 1);
}
