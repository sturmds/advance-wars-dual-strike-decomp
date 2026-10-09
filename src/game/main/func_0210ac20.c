#include "types.h"

extern u16 data_022a9878;
void func_0210b470(int, void *, int);

void func_0210ac20(int a0)
{
    int t0 = data_022a9878 | a0;
    data_022a9878 = t0;
    func_0210b470(a0, &data_022a9878, t0);
}
