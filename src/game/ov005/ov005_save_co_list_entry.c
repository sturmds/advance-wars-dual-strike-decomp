#include "types.h"
#include "game/play_records.h"

extern u8 data_ov005_0236a48c[33]; /* CO id of each list entry */

/* Saves the CO of list entry `entry` in gPlayRecords, so the CO list
 * screen opens on it next time (Ov005_FindSavedCoListEntry). */
u8 Ov005_SaveCoListEntry(int entry) {
  gPlayRecords.wallpaper = data_ov005_0236a48c[entry];
  return data_ov005_0236a48c[entry];
}
