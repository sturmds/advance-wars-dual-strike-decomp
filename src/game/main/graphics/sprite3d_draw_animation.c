#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_022a7c5c.h"

/* colour cycles: 15 colours, repeated so any start reads 15 */
const u16 data_02139d10[32] = {
#include "data/main/data_02139d10.inc.c"
};
const u16 data_02139d50[32] = {
#include "data/main/data_02139d50.inc.c"
};
const u16 data_02139d90[288] = {
#include "data/main/data_02139d90.inc.c"
};
const u8 data_02139fd0[2048] = {
#include "data/main/data_02139fd0.inc.c"
};
static const char data_0213a7d0[4108] = {
#include "data/main/data_0213a7d0.inc.c"
};

int MainLoop_GetFrameCount(void);
void func_020224ec(void);
BOOL SpriteFile_Draw3d(const void* file, int animation, int time,
                       const VecFx32* pos, u32 tileBase, u32 unk28,
                       int drawFlag);

/* Draws the current frame of an animation of the data_0213a7d0 sprite file
 * at pos, timed by the main loop frame count. */
void Sprite3d_DrawAnimation(const VecFx32* pos, int animation) {
  Unk_022a7c5c* gfx;

  func_020224ec();
  gfx = data_022a7c5c;
  SpriteFile_Draw3d(data_0213a7d0, animation, MainLoop_GetFrameCount(), pos,
                    gfx->unk_40, (u32)gfx->unk_44, 0);
}
