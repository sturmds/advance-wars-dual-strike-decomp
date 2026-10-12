#include "types.h"
#include "game/screen_other_map.h"

/* TRUE when the current screen is the one data_027c0288.unk_00 points to
 * and mode is not 3. */
BOOL Screen0_AllowsMode(int mode) {
  if (data_027c027c != data_027c0288.unk_00 || mode == 3) {
    return FALSE;
  }
  return TRUE;
}
