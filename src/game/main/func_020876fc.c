#include "types.h"

void LinkMenu_PushSelectEntry(int a0);

void func_020876fc(u8 *a0)
{
    LinkMenu_PushSelectEntry((*a0 - 12) & 255);
}
