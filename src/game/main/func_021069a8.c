#include "types.h"

int SND_GetCurrentCommandTag(void);
void func_021102b0(int);
int func_02110220(int);

int func_021069a8(void)
{
    int t0 = SND_GetCurrentCommandTag();
    func_021102b0(1);
    return func_02110220(t0);
}
