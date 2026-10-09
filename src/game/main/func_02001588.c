#include "types.h"

void Overlay_Unload(int);
int Overlay_Load(int);

int func_02001588(void)
{
    Overlay_Unload(6);
    return Overlay_Load(1);
}
