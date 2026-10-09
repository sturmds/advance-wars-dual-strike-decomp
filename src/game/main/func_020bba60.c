#include "types.h"

int Weather_StartEffect(int);
int MenuButtons_CloseAll(int);
int func_020b9c68(int);

int func_020bba60(void)
{
    int t0 = Weather_StartEffect(1);
    int t1 = MenuButtons_CloseAll(t0);
    return func_020b9c68(t1);
}
