#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

int func_0207bd94(void);
int Ov003_IsPhase0OrAbove6(void);

#define STATE_BYTE_1D6 (((u8*)&data_ov003_02364928->unk_1d6)[0])
#define STATE_BYTE_1D7 (((u8*)&data_ov003_02364928->unk_1d6)[1])

/* Copies Ov003State.nextPhase into phase, then sets
 * the bytes at 0x1d7 (1 when Unk_02160ac0.unk_7c is set and func_0207bd94
 * returns 0) and 0x1d6 (1 when Ov003_IsPhase0OrAbove6 returns 0). */
void Ov003_AdvancePhase(void) {
  Ov003State* state = data_ov003_02364928;

  state->phase = state->nextPhase;
  if (data_02160ac0->unk_7c != 0 && func_0207bd94() == 0) {
    STATE_BYTE_1D7 = 1;
  } else {
    STATE_BYTE_1D7 = 0;
  }
  if (Ov003_IsPhase0OrAbove6() == 0) {
    STATE_BYTE_1D6 = 1;
  } else {
    STATE_BYTE_1D6 = 0;
  }
}
