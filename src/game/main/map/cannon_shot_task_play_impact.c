#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/cannon_shot_task.h"
#include "game/screen_flash.h"
#include "game/sprite_anim.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;

SpriteAnim3dTask* SpriteAnim3dTask_StartAlt(const void* file,
                                            const VecFx32* pos, u32 tileBase,
                                            u32 unk28, int animation,
                                            int layer);
void Sound_PlaySe(u32 soundId);
void ScreenShake_Start(int script, int frames, Task* parent);
u8* CannonShot_GetAnimFile(
    int kind); /* shot animation file of the cannon kind */
int CannonShot_GetFacingAnim(
    u32 facing); /* first animation for the cannon's facing */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Step of the cannon shot script: plays the impact animation on the target
 * tile (3D or sprite, as the screen needs), with sound 169, a screen shake
 * and a flash. */
void CannonShotTask_PlayImpact(CannonShotTask* task) {
  VecFx32 pos;
  int x;
  int y;

  if (data_027c027c == data_027c0288.unk_00) {
    pos.x = (task->targetX * 16 + 8) * 0x2000 / 256;
    pos.y = (task->targetY * 16 + 16) * 0x6000 / 768;
    pos.z = 8;
    SpriteAnim3dTask_StartAlt(CannonShot_GetAnimFile(task->cannonKind), &pos,
                              (u32)data_022a7c5c->unk_68,
                              (u32)data_022a7c5c->unk_6c,
                              CannonShot_GetFacingAnim(task->facing) + 2, 1);
  } else {
    x = task->targetX * 16 + 8 - STATE->unk_1c4->scrollX;
    y = task->targetY * 16 + 16 - STATE->unk_1c4->scrollY;
    SpriteAnimTask_Start(CannonShot_GetAnimFile(task->cannonKind), x, y, 0x3194,
                         CannonShot_GetFacingAnim(task->facing) + 2, 1);
  }
  Sound_PlaySe(169);
  ScreenShake_Start(1, 20, (Task*)task);
  ScreenFlash_Start(2, 0, 1, (Task*)task);
}
