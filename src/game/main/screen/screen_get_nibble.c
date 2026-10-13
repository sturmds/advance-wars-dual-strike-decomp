#include "types.h"
#include "game/Unk_0216e2b0.h"
#include "game/screen_other_map.h"

/* Reads the current screen's nibble of data_0216e2b0 byte 0: low nibble for
 * the main screen, high nibble for the sub screen. 1 means the screen is
 * hidden. */
int Screen_GetNibble(void) {
  if (data_027c027c == data_027c0288.unk_00) {
    return (u32)(data_0216e2b0.unk_00 << 28) >> 28;
  }
  return (u32)(data_0216e2b0.unk_00 << 24) >> 28;
}
