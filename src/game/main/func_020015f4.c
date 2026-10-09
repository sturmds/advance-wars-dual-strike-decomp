#include "types.h"

void Overlay_Unload(int);
int Overlay_Load(int);

int func_020015f4(void)
{
    Overlay_Unload(1);
    return Overlay_Load(5);
}
