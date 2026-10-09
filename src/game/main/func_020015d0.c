#include "types.h"

void Overlay_Unload(int);
int Overlay_Load(int);

int func_020015d0(void)
{
    Overlay_Unload(5);
    return Overlay_Load(1);
}
