#include "types.h"

int Sound_PlaySe(int);
int Menu_DrawSelectedHelpText(int);

int func_020892f0(void)
{
    int t0 = Sound_PlaySe(0x10002);
    return Menu_DrawSelectedHelpText(t0);
}
