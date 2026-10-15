#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/black_bomb_blast_task.h"

extern u8 data_0213d11c[];

void Units_DamageAroundTile(int x, int y, int damage, int range);
void SpriteAnim3dTask_StartAlt(void* anim, VecFx32* pos, void* gfx,
                               void* palette, int a, int b);
struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);
void Sound_PlaySe(u32 soundId);

/* Blast task step: takes task->damage off the units on tile offset list 3
 * around the blast tile (Units_DamageAroundTile), starts the explosion animation there, as a 3D
 * object on the screen that shows the 3D engine and as a sprite on the
 * other, and plays sound 172. */
void BlackBombBlast_Hit(BlackBombBlastTask* task) {
  VecFx32 pos;

  Units_DamageAroundTile(task->tileX, task->tileY, task->damage, 3);
  if (data_027c027c == data_027c0288.unk_00) {
    pos.x = (task->pixelX * 0x2000) / 256;
    pos.y = (task->pixelY * 0x6000) / 768;
    pos.z = 8;
    SpriteAnim3dTask_StartAlt(data_0213d11c, &pos, data_022a7c5c->unk_68,
                              data_022a7c5c->unk_6c, 0, 1);
  } else {
    SpriteAnimTask_Start(
        data_0213d11c, task->pixelX - MAP_STATE->unk_1c4->scrollX,
        task->pixelY - MAP_STATE->unk_1c4->scrollY, task->sprite, 0, 1);
  }
  Sound_PlaySe(172);
}
