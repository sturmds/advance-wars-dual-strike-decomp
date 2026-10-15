#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0216e2b0.h"

/* Sets window 1's rows from the top mark (unk_02) to the line before the
 * bottom mark (unk_03) in data_0216e2b0. */
void ScreenWipe_EaseUpSetWindow1Rows(void) {
  data_027c0280->win1Top = data_0216e2b0.unk_02;
  data_027c0280->win1Bottom = data_0216e2b0.unk_03 - 1;
}
