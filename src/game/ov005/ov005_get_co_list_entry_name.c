#include "types.h"
#include "game/co_data.h"
#include "game/wallpaper_task.h"

extern u8 data_ov005_0236a48c[];

/* Text id of an entry label: the default wallpaper and the three extra
 * ones have fixed texts, the others show the name of the CO whose wallpaper
 * data_ov005_0236a48c holds (bit 7 is a flag). Entry i holds wallpaper i
 * up to the last extra one, so the entry number stands for the wallpaper. */
u32 Ov005_GetCoListEntryName(int entry) {
  if (entry == 0) {
    return 0x80000031;
  }
  if (entry == WALLPAPER_EXTRA_FIRST) {
    return 0x80000033;
  }
  if (entry == WALLPAPER_EXTRA_FIRST + 1) {
    return 0x80000034;
  }
  if (entry == WALLPAPER_EXTRA_FIRST + 2) {
    return 0x80000032;
  }
  return gCoTable[data_ov005_0236a48c[entry] & 0x7f].name.id;
}
