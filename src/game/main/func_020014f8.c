#include "types.h"

void Overlay_Unload(int);
int Overlay_Load(int);

int func_020014f8(void)
{
    Overlay_Unload(3);
    return Overlay_Load(1);
}
