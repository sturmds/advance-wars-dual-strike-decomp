#include "types.h"
#include "game/sprite_anim.h"
#include "game/capture_picture_task.h"

extern const u8 data_0214a670[];
extern s16 gSineTable[];

SpriteAnim* SpriteAnim_Create(const void* file, int a, int b);
void SpriteAnim_SetAnimation(void* sprite, int animation);
s32 HwDiv32(s32 numer, s32 denom);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);

/* Script step that starts the task shown when a capture completes: creates
 * its sprite, sets affine matrix 0 to the unrotated, full-size matrix
 * (CaptureDone_Draw then scales it) and clears the frame count. */
void CaptureDone_Init(CaptureDoneTask* task) {
  int sinValue;
  int cosValue;

  task->sprite = SpriteAnim_Create(data_0214a670, 0, 2);
  SpriteAnim_SetAnimation(task->sprite, 1);
  task->sprite->tileBase = (3 << 12) | 0x194; /* tile 0x194, palette 3 */
  cosValue = gSineTable[64] * 16;
  sinValue = gSineTable[0];
  Oam_SetAffineParams(0, HwDiv32(cosValue, 256), HwDiv32(-sinValue * 16, 256),
                      HwDiv32(sinValue * 16, 256), HwDiv32(cosValue, 256));
  task->frame = 0;
}
