#include "types.h"
#include "game/Unk_02290118.h"
#include "game/screen_other_map.h"

BOOL Profile_IsInUse(int profile);
int func_0210391c(void);

static inline u16 CurrentMap_GetMapId(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  u16 mapId;

  Screen_SetCurrentGfx((&data_027c0288.unk_00)[data_027c0284]);
  saved027c = data_027c027c;
  Screen_SetCurrent((&data_027c0288.unk_00)[data_027c0284]);
  mapId = data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  return mapId;
}

/* Called at the end of a campaign battle (BattleScore_Compute) with the current
 * profile, which it does not use, and a value. Keeps the larger of value and
 * data_02290118.unk_0e in unk_0e (0 unless unk_05 is set) and returns the
 * smaller. Raises unk_0e of every profile in use with the same unk_01,
 * hardMode and current map (unk_0c) to that larger value. On map 0xf8, unless
 * data_02290118.unk_08 is set, also sets unk_08 of the profiles with the same
 * unk_01 and hardMode. Clears unk_05. Needed to match: the second loop has its
 * own pointer, mapId is u32. */
int ProfileHeaders_UpdateBest(int profile, int value) {
  u32 mapId;
  int i;
  int best;
  ProfileHeader* header;
  ProfileHeader* entry;

  mapId = CurrentMap_GetMapId();

  if (!data_02290118.unk_05) {
    data_02290118.unk_0e = 0;
  }
  if (value < data_02290118.unk_0e) {
    best = data_02290118.unk_0e;
  } else {
    best = value;
    value = data_02290118.unk_0e;
  }
  if (data_02290118.unk_05) {
    data_02290118.unk_0e = best;
  } else {
    data_02290118.unk_0e = 0;
  }
  for (i = 0, header = data_02290a60; i < 3; i++, header++) {
    if (Profile_IsInUse(i) && header->unk_05 &&
        header->unk_01 == data_02290118.unk_01 && header->unk_0c == mapId &&
        header->hardMode == data_02290118.hardMode && header->unk_0e < best) {
      header->unk_0e = best;
    }
  }
  for (i = 0, entry = data_02290a60; i < 3; i++, entry++) {
    if (Profile_IsInUse(i) && entry->unk_01 == data_02290118.unk_01 &&
        entry->hardMode == data_02290118.hardMode && mapId == 0xf8 &&
        !func_0210391c()) {
      entry->unk_08 = 1;
    }
  }
  data_02290118.unk_05 = 0;
  return value;
}
