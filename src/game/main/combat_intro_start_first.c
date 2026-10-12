#include "types.h"
#include "game/task.h"
#include "game/unlocks.h"

extern const TaskScriptCmd data_021618b0[];

/* Plays the script of data_021618b0 as a child task once (game flag 54). */
void CombatIntro_StartFirst(Task* parent) {
  if (GameFlag_IsSet(54)) {
    return;
  }
  GameFlag_Set(54, 1);
  Task_CreateChild(data_021618b0, parent);
}
