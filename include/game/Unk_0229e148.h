#ifndef GAME_UNK_0229E148_H
#define GAME_UNK_0229E148_H

#include "types.h"
#include "game/Unk_02160ac0.h"

/* data_0229e148 (main bss): the parts of the combat setup at data_02160ac0
 * that the save keeps. CombatSetup_SaveToBuffer fills it, func_0208bf40
 * copies it back. */
typedef struct Unk_0229e148 {
  u8 cells[COMBAT_CELL_COUNT]; /* Unk_02160ac0.cells, copied as bytes */
  u8 cellCount;
  u8 co;
  u8 unk_16; /* Unk_02160ac0.unk_7d */
  u8 unk_17; /* Unk_02160ac0.unk_84 */
} Unk_0229e148;

typedef char Unk_0229e148_size_check[sizeof(Unk_0229e148) == 0x18 ? 1 : -1];

#endif
