#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_0215777c[];
extern u16 data_021579ac[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_0200e32c(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_0215777c[1];
  {
    u16 x = data_021579ac[0];
    u16 y = data_021579ac[1];
    u16 z = data_021579ac[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_0215777c[0];
  {
    u16 x = data_021579ac[3];
    u16 y = data_021579ac[4];
    u16 z = data_021579ac[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_0215777c[3];
  {
    u16 x = data_021579ac[6];
    u16 y = data_021579ac[7];
    u16 z = data_021579ac[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_0215777c[2];
  {
    u16 x = data_021579ac[9];
    u16 y = data_021579ac[10];
    u16 z = data_021579ac[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
