#include "types.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577ac[4];
extern u16 data_0215797c[12];

void Gx_SendPlainQuad_02011058(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577ac[3];
  {
    u16 x = data_0215797c[0];
    u16 y = data_0215797c[1];
    u16 z = data_0215797c[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577ac[2];
  {
    u16 x = data_0215797c[3];
    u16 y = data_0215797c[4];
    u16 z = data_0215797c[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577ac[1];
  {
    u16 x = data_0215797c[6];
    u16 y = data_0215797c[7];
    u16 z = data_0215797c[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577ac[0];
  {
    u16 x = data_0215797c[9];
    u16 y = data_0215797c[10];
    u16 z = data_0215797c[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
