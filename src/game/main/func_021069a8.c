#include "types.h"

int SND_GetCurrentCommandTag(void);
void func_021102b0(int);
int SND_WaitForCommandProc(int);

int func_021069a8(void)
{
    int t0 = SND_GetCurrentCommandTag();
    func_021102b0(1);
    return SND_WaitForCommandProc(t0);
}
