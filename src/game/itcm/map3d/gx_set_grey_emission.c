#include "types.h"

#define GXFIFO_DIF_AMB ((volatile u32*)0x040004c0)
#define GXFIFO_SPE_EMI ((volatile u32*)0x040004c4)

/* Sets the material for the vertices that follow: specular white, emission
 * a grey of level (0-31) in each colour channel, diffuse and ambient white. */
void Gx_SetGreyEmission(u32 level) {
  u16 grey = level | (level << 5) | (level << 10);

  *GXFIFO_SPE_EMI = 0x7fff | (grey << 16);
  *GXFIFO_DIF_AMB = 0x7fff7fff;
}
