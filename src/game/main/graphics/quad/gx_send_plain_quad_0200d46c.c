#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_0215778c[];
extern u16 data_02157a84[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_0200d46c(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_0215778c[2];
  {
    u16 x = data_02157a84[0];
    u16 y = data_02157a84[1];
    u16 z = data_02157a84[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_0215778c[3];
  {
    u16 x = data_02157a84[3];
    u16 y = data_02157a84[4];
    u16 z = data_02157a84[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_0215778c[0];
  {
    u16 x = data_02157a84[6];
    u16 y = data_02157a84[7];
    u16 z = data_02157a84[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_0215778c[1];
  {
    u16 x = data_02157a84[9];
    u16 y = data_02157a84[10];
    u16 z = data_02157a84[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
