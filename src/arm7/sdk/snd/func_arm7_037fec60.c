#include "types.h"
#include "sdk/svc.h"

/* BIOS call 0x1c (GetVolumeTable, by the caller). */

int func_arm7_037fec60(int index) { return Bios_VolumeTableEntry_Arm7(index); }
