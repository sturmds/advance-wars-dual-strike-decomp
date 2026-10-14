#include "types.h"
#include "game/play_records.h"

extern u8 data_ov005_0236a48c[];

int Ov005_FindSavedCoListEntry(void) {
  u8 slot = gPlayRecords.wallpaper;
  int i;

  for (i = 0; i < 33; i++) {
    if (data_ov005_0236a48c[i] == slot) {
      return i;
    }
  }
  return i;
}
