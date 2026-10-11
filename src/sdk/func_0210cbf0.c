#include "types.h"
#include "game/song.h"

void IrqSlot_CallAndClear(int length);

void func_0210cbf0(void)
{
    IrqSlot_CallAndClear(2);
}
