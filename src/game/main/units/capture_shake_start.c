#include "types.h"
#include "game/capture_picture_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168df0[];
extern const TaskScriptCmd data_02168dc8[];

CapturePictureTask* Task_FindByScript(const TaskScriptCmd* script);
void SpriteAnim_SetAnimation(void* sprite, int animation);
void Sound_PlaySe(u32 soundId);

/* Script step of the capture count task: switches the picture's army
 * sprite to animation 1, starts a child task (script data_02168dc8, probably
 * the shake of CaptureShake_Update) and plays sound 151. */
void CaptureShake_Start(Task* task) {
  CapturePictureTask* picture = Task_FindByScript(data_02168df0);

  SpriteAnim_SetAnimation(picture->armySprite, 1);
  Task_CreateChild(data_02168dc8, task);
  Sound_PlaySe(151);
}
