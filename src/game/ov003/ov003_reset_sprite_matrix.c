#include "types.h"
#include "game/ov003_unit.h"
#include "game/oam_buffer.h"
#include "game/math.h"

/* Loads sprite matrix 0 with the rotation for angle 0 of the sine table
 * (the same loading as func_02049ddc, with divisor 256) and clears the next
 * mode. */
void Ov003_ResetSpriteMatrix(void) {
  int cosValue = gSineTable[64] * 16;
  int sinValue = gSineTable[0];

  Oam_SetAffineParams(0, HwDiv32(cosValue, 256), HwDiv32(-sinValue * 16, 256),
                      HwDiv32(sinValue * 16, 256), HwDiv32(cosValue, 256));
  data_ov003_02364928->nextPhase = 0;
}
