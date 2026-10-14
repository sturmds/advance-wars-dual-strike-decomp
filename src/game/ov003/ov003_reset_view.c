#include "types.h"
#include "game/ov003_unit.h"
#include "game/Ov003Fx.h"

/* Resets the scroll position (state 0x1d0 and 0x1d2, which Ov003_DrawIconAt
 * compares screen positions against) and clears the flag fields of
 * data_ov003_02364a84. */
void Ov003_ResetView(void) {
  data_ov003_02364928->scrollX = 0;
  data_ov003_02364928->scrollY = 0;
  data_ov003_02364a84->unk_0b_0 = 0;
  data_ov003_02364a84->unk_0b_2 = 0;
  data_ov003_02364a84->unk_0a_0 = 0;
  data_ov003_02364a84->unk_0a_2 = 0;
  data_ov003_02364a84->unk_0b_0 = 0;
  data_ov003_02364a84->unk_0b_2 = 0;
  data_ov003_02364a84->unk_0c_0 = 0;
  data_ov003_02364a84->unk_0c_4 = 0;
}
