#include "types.h"
#include "game/task.h"
#include "game/unlocks.h"

extern const TaskScriptCmd data_021618c8[];

/* Plays the script of data_021618c8 as a child task once (game flag 55). */
void CombatIntro_StartSecond(Task* parent) {
  if (GameFlag_IsSet(55)) {
    return;
  }
  GameFlag_Set(55, 1);
  Task_CreateChild(data_021618c8, parent);
}
