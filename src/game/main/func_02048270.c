#include "types.h"

int Sound_PlaySe(int);
int Menu_Dismiss(int);

int func_02048270(void)
{
    int t0 = Sound_PlaySe(0x10006);
    return Menu_Dismiss(t0);
}
