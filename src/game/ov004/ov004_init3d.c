#include "types.h"

#define GXFIFO_MTX_MODE ((volatile u32*)0x04000440)
#define GXFIFO_MTX_PUSH ((volatile u32*)0x04000444)
#define GXFIFO_MTX_IDENTITY ((volatile u32*)0x04000454)
#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_POLYGON_ATTR ((volatile u32*)0x040004a4)
#define GXFIFO_DIF_AMB ((volatile u32*)0x040004c0)
#define GXFIFO_SPE_EMI ((volatile u32*)0x040004c4)
#define GXFIFO_LIGHT_VECTOR ((volatile u32*)0x040004c8)
#define GXFIFO_LIGHT_COLOR ((volatile u32*)0x040004cc)

void G3X_Reset(void);
void G3i_LookAt_(const void* a, const void* b, void* c, int d, int e);

extern u8 data_ov004_022bdcec[];
extern u8 data_ov004_022bdce0[];
static u8 data_ov004_022bf140[0xc]; /* size: up to the next symbol */

/* Sets up the 3D engine for overlay 4: a projection, then light 0 and the
 * material colours, an identity matrix, and the polygon attributes. */
void Ov004_Init3d(void) {
  G3X_Reset();
  G3i_LookAt_(data_ov004_022bdcec, data_ov004_022bdce0, data_ov004_022bf140, 1,
              0);
  *GXFIFO_LIGHT_VECTOR = 0x1ff00000;
  *GXFIFO_LIGHT_COLOR = 0x7fff;
  *GXFIFO_DIF_AMB = 0x7fffffff;
  *GXFIFO_SPE_EMI = 0x4210;
  *GXFIFO_MTX_PUSH = 0;
  *GXFIFO_MTX_MODE = 3;
  *GXFIFO_MTX_IDENTITY = 0;
  *GXFIFO_MTX_MODE = 2;
  *GXFIFO_NORMAL = 0x20000000;
  *GXFIFO_POLYGON_ATTR = 0x001f0081;
}
