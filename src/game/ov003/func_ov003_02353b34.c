#include "types.h"

#define GXFIFO_DIF_AMB (*(volatile u32*)0x040004c0)
#define GXFIFO_SPE_EMI (*(volatile u32*)0x040004c4)
#define GXFIFO_LIGHT_VECTOR (*(volatile u32*)0x040004c8)
#define GXFIFO_LIGHT_COLOR (*(volatile u32*)0x040004cc)

void func_ov003_02353b34(void) {
  GXFIFO_LIGHT_VECTOR = 0x1ff00000;
  GXFIFO_LIGHT_COLOR = 0x7fff;
  GXFIFO_DIF_AMB = 0x7fffffff;
  GXFIFO_SPE_EMI = 0x4210;
}
