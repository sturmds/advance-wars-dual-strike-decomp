#include "types.h"

void TileSpriteTask_Start(
    int x, int y, int tile,
    int kind); /* u8, u16 in its file; needed to match: no narrowing here */

/* Starts the CO power effect sprite on tile (x, y), with the front CO's
 * sound for kind (the power block, PlayerCo.unk_12). */
void PowerEffect_Start(int x, int y, int kind) {
  TileSpriteTask_Start(x, y, 0x5194, kind);
}
