#include "types.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_0215781c[4];
extern u16 data_02157994[12];

void Gx_SendPlainQuad_0200e418(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_0215781c[1];
  {
    u16 x = data_02157994[0];
    u16 y = data_02157994[1];
    u16 z = data_02157994[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_0215781c[0];
  {
    u16 x = data_02157994[3];
    u16 y = data_02157994[4];
    u16 z = data_02157994[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_0215781c[3];
  {
    u16 x = data_02157994[6];
    u16 y = data_02157994[7];
    u16 z = data_02157994[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_0215781c[2];
  {
    u16 x = data_02157994[9];
    u16 y = data_02157994[10];
    u16 z = data_02157994[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
