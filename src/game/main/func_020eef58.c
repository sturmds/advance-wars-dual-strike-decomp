#include "types.h"

int ActingUnit_SaveStartPos(void);
int ActingUnit_FinishAction(int);
int ActingUnitStart_Restore(int);
int func_020b9d48(int);

int func_020eef58(void)
{
    int t0 = ActingUnit_SaveStartPos();
    int t1 = ActingUnit_FinishAction(t0);
    int t2 = ActingUnitStart_Restore(t1);
    return func_020b9d48(t2);
}
