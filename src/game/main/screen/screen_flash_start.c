#include "types.h"
#include "game/screen_flash.h"

extern const TaskScriptCmd data_02157480[];

/* Starts a white flash of the current screen, under parent or on task list 3. */
Task* ScreenFlash_Start(int fadeInFrames, int holdFrames, int fadeOutFrames,
                        Task* parent) {
  ScreenFlashTask* flash;

  if (parent != NULL) {
    flash = (ScreenFlashTask*)Task_CreateChild(data_02157480, parent);
  } else {
    flash = (ScreenFlashTask*)Task_Create(data_02157480, TASK_LIST(3));
  }
  flash->fadeInFrames = fadeInFrames;
  flash->holdFrames = holdFrames;
  flash->fadeOutFrames = fadeOutFrames;
  return (Task*)flash;
}
