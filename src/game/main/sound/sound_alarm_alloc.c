#include "types.h"
#include "game/sound.h"

/* Takes the lowest free alarm (0 to 7) and returns its number, or -1 when
 * all eight are in use. */
int SoundAlarm_Alloc(void) {
  int i;
  u32 bit = 1;

  for (i = 0; i < 8; i++, bit <<= 1) {
    if ((data_022a8048 & bit) == 0) {
      data_022a8048 |= bit;
      return i;
    }
  }
  return -1;
}
