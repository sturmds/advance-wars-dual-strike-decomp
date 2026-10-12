#include "types.h"
#include "sdk/gx.h"

void ScriptSlot_RunAll(void);
void func_ov002_02350590(void);

/* Per-frame callback of the battle screen (registered by Battle_StartScreen):
 * it ends by writing the 3D engine's SWAP_BUFFERS. func_ov002_02350590 is
 * called without arguments, so its declared parameters are probably wrong. */
void Battle_DrawFrame(void) {
  ScriptSlot_RunAll();
  func_ov002_02350590();
  *REG_SWAP_BUFFERS = 3;
}
