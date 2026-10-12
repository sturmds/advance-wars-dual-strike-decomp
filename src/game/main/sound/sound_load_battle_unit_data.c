#include "types.h"
#include "game/unit_stats.h"

void Sound_RestoreHeapState(
    int unused); /* frees the sound heap back to its last saved level */
void Sound_LoadArchiveItems(int item1,
                            int item2); /* loads two sound archive items */

/* Two sound archive items per unit type, as pairs; UNIT_NONE has none.
 * Needed to match: indexed as a flat array, not as [type][2]. */
extern u32 gUnitSoundItems[UNIT_TYPE_COUNT * 2];

/* Replaces what was loaded after the last saved sound heap level with the
 * battle sounds (items 2 and 0x31) and the sounds of the two unit types.
 * Called by the ov002 battle animation with the types of its two sides. */
void Sound_LoadBattleUnitData(int unitType1, int unitType2) {
  Sound_RestoreHeapState(0);
  Sound_LoadArchiveItems(2, 0x31);
  Sound_LoadArchiveItems(gUnitSoundItems[unitType1 * 2],
                         gUnitSoundItems[unitType1 * 2 + 1]);
  Sound_LoadArchiveItems(gUnitSoundItems[unitType2 * 2],
                         gUnitSoundItems[unitType2 * 2 + 1]);
}
