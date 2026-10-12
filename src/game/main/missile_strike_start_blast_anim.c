#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_022a7c5c.h"
#include "game/missile_strike_task.h"

extern u8 data_0213bda0[];

void SpriteAnim3dTask_StartAlt(const void* file, const VecFx32* pos,
                               void* tileBase, void* palette, int animation,
                               int layer);

/* Script step of data_02169088: starts the animation data_0213bda0 at the
 * landing pixel as a 3D object (what it shows is a guess: the blast). */
void MissileStrike_StartBlastAnim(MissileStrikeTask* task) {
  VecFx32 pos;

  int y = task->pixelY;
  int x = task->pixelX;

  pos.x = x * 0x2000 / 256;
  pos.y = y * 0x6000 / 768;
  pos.z = 8;
  SpriteAnim3dTask_StartAlt(data_0213bda0, &pos, data_022a7c5c->unk_68,
                            data_022a7c5c->unk_6c, 0, 0);
}
