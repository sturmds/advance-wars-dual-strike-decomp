#include "types.h"

void PopupMenu_PopState(void);
int Sound_PlaySe(int);

int func_ov006_023578e8(void)
{
    PopupMenu_PopState();
    return Sound_PlaySe(0x10009);
}
