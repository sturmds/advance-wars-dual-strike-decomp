#include "types.h"

/* BIOS call 0x1b (GetPitchTable, by the caller). */
extern int Bios_PitchTableEntry_Arm7(int index);

int func_arm7_037fed94(int index) { return Bios_PitchTableEntry_Arm7(index); }
