#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/Unk_0229e148.h"

extern Unk_0229e148 data_0229e148;

void CombatSetup_ResetDefaults(void);
void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);

/* Copies the 24 bytes at data_0229e148 into the block data_02160ac0 points to:
 * bytes 0-19 as a block, bytes 20-23 to offsets 0x18, 0x19, 0x7d and 0x84.
 * CombatSetup_SaveToBuffer reads them back. What the block holds is not known. */
void func_0208bf40(void) {
  CombatSetup_ResetDefaults();
  Mem_CopyUnits(&data_0229e148, data_02160ac0, sizeof(data_02160ac0->cells), 8);
  data_02160ac0->cellCount = data_0229e148.cellCount;
  data_02160ac0->co = data_0229e148.co;
  data_02160ac0->unk_7d = data_0229e148.unk_16;
  data_02160ac0->unk_84 = data_0229e148.unk_17;
}
