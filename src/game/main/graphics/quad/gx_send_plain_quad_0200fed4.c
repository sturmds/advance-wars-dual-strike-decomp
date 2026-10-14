#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_0215779c[];
extern u16 data_0215791c[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_0200fed4(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_0215779c[2];
  {
    u16 x = data_0215791c[0];
    u16 y = data_0215791c[1];
    u16 z = data_0215791c[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_0215779c[3];
  {
    u16 x = data_0215791c[3];
    u16 y = data_0215791c[4];
    u16 z = data_0215791c[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_0215779c[0];
  {
    u16 x = data_0215791c[6];
    u16 y = data_0215791c[7];
    u16 z = data_0215791c[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_0215779c[1];
  {
    u16 x = data_0215791c[9];
    u16 y = data_0215791c[10];
    u16 z = data_0215791c[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
