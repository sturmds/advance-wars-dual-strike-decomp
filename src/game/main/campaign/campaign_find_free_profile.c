#include "types.h"
#include "game/play_records.h"

void func_0210394c(void);

/* Byte `field` of the 16-byte profile record gPlayRecords.unk_348[profile].
 * Written as a row of a 16-byte array counted from gPlayRecords, with the
 * offset in the column; needed to match. */
#define PROFILE_BYTE(profile, field) \
  ((u8(*)[16]) &                     \
   gPlayRecords)[profile][(u32) & ((PlayRecords*)0)->unk_348 + (field)]

/* Returns the first of profile slots 0 to 2 that no saved profile uses, or 3
 * when all are used. */
int Campaign_FindFreeProfile(void) {
  int taken[4];
  int i;
  int slot;

  for (i = 0; i < 4; i++) {
    taken[i] = 0;
  }
  for (i = 0; i < 3; i++) {
    if (PROFILE_BYTE(i, 3) != 0) {
      taken[PROFILE_BYTE(i, 1)] = 1;
    }
  }
  for (i = 0; i < 3; i++) {
    if (taken[i] == 0) {
      slot = i;
      break;
    }
  }
  if (i == 3) {
    slot = 3;
  }
  func_0210394c();
  return slot;
}
