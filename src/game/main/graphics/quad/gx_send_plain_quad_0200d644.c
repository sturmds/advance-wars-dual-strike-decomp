#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577bc[];
extern u16 data_021579dc[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_0200d644(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577bc[2];
  {
    u16 x = data_021579dc[0];
    u16 y = data_021579dc[1];
    u16 z = data_021579dc[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577bc[3];
  {
    u16 x = data_021579dc[3];
    u16 y = data_021579dc[4];
    u16 z = data_021579dc[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577bc[0];
  {
    u16 x = data_021579dc[6];
    u16 y = data_021579dc[7];
    u16 z = data_021579dc[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577bc[1];
  {
    u16 x = data_021579dc[9];
    u16 y = data_021579dc[10];
    u16 z = data_021579dc[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
