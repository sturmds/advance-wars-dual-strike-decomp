#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0288 data_027c0288;
extern u8 data_0216e2b0;

/* data_0216e2b0 byte 0 keeps one 4-bit value per screen: low nibble for the
 * main screen, high nibble for the sub screen. Sets the current screen's. */
void Screen_SetNibble(int value) {
  if (data_027c027c == data_027c0288.unk_00) {
    data_0216e2b0 = (data_0216e2b0 & ~15) | (value & 15);
  } else {
    data_0216e2b0 = (data_0216e2b0 & ~0xf0) | ((value & 15) << 4);
  }
}
