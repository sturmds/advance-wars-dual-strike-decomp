#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_022a7c5c.h"

extern char data_0213a7d0[];

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
