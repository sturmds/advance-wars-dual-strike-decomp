#include "types.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577fc[4];
extern u16 data_021578a4[12];

void Gx_SendPlainQuad_0200dbcc(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577fc[1];
  {
    u16 x = data_021578a4[0];
    u16 y = data_021578a4[1];
    u16 z = data_021578a4[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577fc[0];
  {
    u16 x = data_021578a4[3];
    u16 y = data_021578a4[4];
    u16 z = data_021578a4[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577fc[3];
  {
    u16 x = data_021578a4[6];
    u16 y = data_021578a4[7];
    u16 z = data_021578a4[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577fc[2];
  {
    u16 x = data_021578a4[9];
    u16 y = data_021578a4[10];
    u16 z = data_021578a4[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
