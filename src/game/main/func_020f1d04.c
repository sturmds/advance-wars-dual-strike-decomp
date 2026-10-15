#include "types.h"
#include "game/task.h"
#include "game/Unk_021694c0.h"
#include "game/math.h"

extern Unk_021694c0* data_021694c0;
extern const TaskScriptCmd data_02169878[];

Unk_021694c0Task* Task_FindByScript(const TaskScriptCmd* script);
void Task_ResumeScript(Unk_021694c0Task* task);
void SpriteAnim_SetAnimation(struct ShowSprite* sprite, int frame);
void EffectScene_SetMode(int value);

void func_020f1d04(Unk_021694c0Task* task) {
  Unk_021694c0Task* other;

  data_021694c0->unk_28 = task->y + Math_Ease(1, 0, 192, task->frame++, 120);
  if (task->frame == 103) {
    other = Task_FindByScript(data_02169878);
    if (other != NULL) {
      SpriteAnim_SetAnimation(other->sprite, 1);
    }
  }
  if (task->frame > 120) {
    EffectScene_SetMode(0);
    Task_ResumeScript(task);
  }
}
