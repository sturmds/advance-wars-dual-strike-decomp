#include "types.h"
#include "game/Unk_02160ac0.h"
#include "game/Unk_0229e148.h"

extern Unk_0229e148 data_0229e148;

void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits);

/* Reverse of func_0208bf40: copies the block data_02160ac0 points to (bytes
 * 0-19, 0x18, 0x19, 0x7d and 0x84) into the 24 bytes at data_0229e148. */
void CombatSetup_SaveToBuffer(void) {
  Mem_CopyUnits(data_02160ac0, &data_0229e148, sizeof(data_02160ac0->cells), 8);
  data_0229e148.cellCount = data_02160ac0->cellCount;
  data_0229e148.co = data_02160ac0->co;
  data_0229e148.unk_16 = data_02160ac0->unk_7d;
  data_0229e148.unk_17 = data_02160ac0->unk_84;
}
