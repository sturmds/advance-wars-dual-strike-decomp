#include "types.h"
#include "game/unit_show_entry.h"
#include "game/unit_show_task.h"
#include "game/unit_show_entry.h"

void Sound_PlaySe(int se);
void Sound_StopSubSe(int se);

void ShowEntry_PlaySound(UnitShowTask* task) {
  u16 sound = gUnitShowEntries[task->entry].sound;

  if (sound == SHOW_SOUND_NONE) {
    return;
  }
  switch (sound) {
    case SHOW_SOUND_SPECIAL:
      UnitShowTask_PlayStepSound(task);
      break;
    case SHOW_SOUND_STOP:
      Sound_StopSubSe(20);
      break;
    default:
      Sound_PlaySe(sound);
      break;
  }
}
