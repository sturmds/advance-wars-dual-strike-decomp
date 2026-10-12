#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_0215bbc8[];

/* Starts the title call task that shows the campaign record number
 * (script data_0215bbc8, with TitleCall_NumberSlideIn). */
void TitleCall_StartNumberTask(void* parentOrList) {
  Task_Create(data_0215bbc8, parentOrList);
}
