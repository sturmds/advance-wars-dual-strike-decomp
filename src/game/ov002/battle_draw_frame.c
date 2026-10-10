#include "types.h"

void func_02017494(void);
void func_ov002_02350590(void);

/* Per-frame callback of the battle screen (registered by Battle_StartScreen):
 * the last write is the 3D engine's SWAP_BUFFERS. func_ov002_02350590 is
 * called without arguments, so its declared parameters are probably wrong. */
void Battle_DrawFrame(void)
{
    func_02017494();
    func_ov002_02350590();
    *((int *)0x4000540) = 3;
}
