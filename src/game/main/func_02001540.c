#include "types.h"

void Overlay_Unload(int);
int Overlay_Load(int);

int func_02001540(void)
{
    Overlay_Unload(2);
    return Overlay_Load(1);
}
