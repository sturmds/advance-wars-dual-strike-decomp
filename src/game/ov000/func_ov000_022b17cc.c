#include "types.h"

int Task_ResumeByScript(void *);
void data_ov000_0234f264(void);
int MenuButtons_CloseAll(int);

int func_ov000_022b17cc(void)
{
    int t0 = Task_ResumeByScript((void *)data_ov000_0234f264);
    return MenuButtons_CloseAll(t0);
}
