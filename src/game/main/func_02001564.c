#include "types.h"

void Overlay_Unload(int);
int Overlay_Load(int);

int func_02001564(void)
{
    Overlay_Unload(1);
    return Overlay_Load(2);
}
