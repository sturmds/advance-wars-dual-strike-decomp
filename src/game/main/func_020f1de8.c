#include "types.h"
#include "game/task.h"
#include "game/screen_flash.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;
extern u8 data_0213b8f0[];

void Sound_PlaySe(u32 soundId);
void ScreenShake_Start(int script, int frames, Task* parent);
void Task_ResumeScript(Unk_021694c0Task* task);
void SpriteAnim_Draw(struct ShowSprite* sprite, int x, int y);
void SpriteAnim_Free(struct ShowSprite* sprite);
struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);

/* Moves the sprite up 8 pixels a frame. At the scene position it is released,
 * and a sprite, a shake and a flash mark the impact. */
void func_020f1de8(Unk_021694c0Task* task) {
  SpriteAnim_Draw(task->sprite, task->x & 0x1ff, task->y & 0xff);
  task->y -= 8;
  if (task->y >= data_021694c0->unk_28) {
    return;
  }
  SpriteAnim_Free(task->sprite);
  SpriteAnimTask_Start(
      data_0213b8f0, data_021694c0->unk_24 + data_021694c0->unk_2c,
      data_021694c0->unk_28 + data_021694c0->unk_30, 0x4170, 5, 0);
  Sound_PlaySe(172);
  ScreenShake_Start(1, 20, (Task*)task);
  ScreenFlash_Start(4, 0, 2, (Task*)task);
  Task_ResumeScript(task);
}
