#include "types.h"

/* BIOS call 0x1c (GetVolumeTable, by the caller). */
extern int func_arm7_03803bec(int index);

int func_arm7_037fec60(int index)
{
    return func_arm7_03803bec(index);
}
