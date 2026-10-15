#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/ex_machina_bolt_task.h"

const u16 gObjRowPalette[] = {
#include "data/main/gObjRowPalette.inc.c"
};
static const u8 data_02134d68[] = {
#include "data/main/data_02134d68.inc.c"
};

void SpriteAnim3dTask_StartAlt(const void* file, const VecFx32* pos,
                               void* tileBase, void* palette, int animation,
                               int layer);
struct SpriteAnimTask* SpriteAnimTask_Start(const void* file, int x, int y,
                                            u32 tileBase, int animation,
                                            int layer);

/* Script step of gExMachinaBoltScript: starts the animation data_02134d68 at the
 * centre of the bolt's tile, in 3D on the screen that shows the 3D engine
 * and as a sprite on the other. What the animation shows is a guess. */
void ExMachinaBolt_PlayAnim(ExMachinaBoltTask* task) {
  VecFx32 pos;
  int x = task->tileX * 16 + 8;
  int y = task->tileY * 16 + 8;

  if (data_027c027c == data_027c0288.unk_00) {
    pos.x = x * 0x2000 / 256;
    pos.y = y * 0x6000 / 768;
    pos.z = 8;
    SpriteAnim3dTask_StartAlt(data_02134d68, &pos, data_022a7c5c->unk_68,
                              data_022a7c5c->unk_6c, 0, 0);
  } else {
    SpriteAnimTask_Start(data_02134d68, x - MAP_STATE->unk_1c4->scrollX,
                         y - MAP_STATE->unk_1c4->scrollY, 0x3194, 0, 0);
  }
}
