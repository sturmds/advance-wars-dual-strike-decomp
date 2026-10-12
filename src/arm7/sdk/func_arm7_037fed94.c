#include "types.h"

/* BIOS call 0x1b (GetPitchTable, by the caller). */
extern int func_arm7_03803be8(int index);

int func_arm7_037fed94(int index)
{
    return func_arm7_03803be8(index);
}
