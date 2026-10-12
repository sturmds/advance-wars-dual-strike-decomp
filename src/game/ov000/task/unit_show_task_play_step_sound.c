#include "types.h"
#include "game/unit_show_entry.h"
#include "game/unit_show_task.h"
#include "game/unit_stats.h"

#define SOUND_SUB_DIVE 118
#define STEP_SOUND (gUnitShowEntries[task->entry].stepSound)

void Sound_PlaySe(int se);
void Sound_PlaySubSe(int se);
int UnitShowTask_GetTerrainSound(UnitShowTask* task);

/* Plays the sound of a step; the entry number equals the unit type. Infantry and Mech
 * play the sound for the terrain under them, when the entry has one; a
 * dived Sub plays its dive sound; the rest play the entry's own sound. */
void UnitShowTask_PlayStepSound(UnitShowTask* task) {
  int se;

  switch (task->entry) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
      if (STEP_SOUND == 0) {
        return;
      }
      se = UnitShowTask_GetTerrainSound(task);
      if (se == 0) {
        return;
      }
      Sound_PlaySe(se);
      return;
    case UNIT_SUB:
      if (task->unit != NULL && (task->unit->flags & MAP_UNIT_DIVED)) {
        Sound_PlaySubSe(SOUND_SUB_DIVE);
        return;
      }
      if (STEP_SOUND == 0) {
        return;
      }
      Sound_PlaySubSe(STEP_SOUND);
      return;
    default:
      if (STEP_SOUND == 0) {
        return;
      }
      Sound_PlaySubSe(STEP_SOUND);
      return;
  }
}
