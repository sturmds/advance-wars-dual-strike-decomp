#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577fc[];
extern u16 data_0215794c[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_02011144(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577fc[3];
  {
    u16 x = data_0215794c[0];
    u16 y = data_0215794c[1];
    u16 z = data_0215794c[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577fc[2];
  {
    u16 x = data_0215794c[3];
    u16 y = data_0215794c[4];
    u16 z = data_0215794c[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577fc[1];
  {
    u16 x = data_0215794c[6];
    u16 y = data_0215794c[7];
    u16 z = data_0215794c[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577fc[0];
  {
    u16 x = data_0215794c[9];
    u16 y = data_0215794c[10];
    u16 z = data_0215794c[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
