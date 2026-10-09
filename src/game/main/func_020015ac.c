#include "types.h"

void Overlay_Unload(int);
int Overlay_Load(int);

int func_020015ac(void)
{
    Overlay_Unload(1);
    return Overlay_Load(6);
}
