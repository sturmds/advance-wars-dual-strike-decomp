#include "types.h"

/* BIOS call 0x1c (GetVolumeTable, by the caller). */
extern int Bios_VolumeTableEntry_Arm7(int index);

int func_arm7_037fec60(int index)
{
    return Bios_VolumeTableEntry_Arm7(index);
}
