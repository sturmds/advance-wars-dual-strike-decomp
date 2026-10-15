#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0216e2b0.h"

/* Sets window 1's rows from the two line marks that WindowWipe_BuildEdgesIn
 * leaves in data_0216e2b0: from the bottom mark (unk_03) to the top mark
 * (unk_02). When the marks meet, window 1 covers all 192 rows. */
void ScreenWipe_EaseDownSetWindow1Rows(void) {
  if (data_0216e2b0.unk_02 == data_0216e2b0.unk_03 - 1) {
    data_027c0280->win1Top = 0;
    data_027c0280->win1Bottom = 192;
  } else {
    data_027c0280->win1Top = data_0216e2b0.unk_03;
    data_027c0280->win1Bottom = data_0216e2b0.unk_02;
  }
}
