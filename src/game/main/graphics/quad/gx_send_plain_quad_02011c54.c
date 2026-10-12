#include "types.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_0215775c[4];
extern u16 data_02157934[12];

void Gx_SendPlainQuad_02011c54(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_0215775c[0];
  {
    u16 x = data_02157934[0];
    u16 y = data_02157934[1];
    u16 z = data_02157934[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_0215775c[1];
  {
    u16 x = data_02157934[3];
    u16 y = data_02157934[4];
    u16 z = data_02157934[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_0215775c[2];
  {
    u16 x = data_02157934[6];
    u16 y = data_02157934[7];
    u16 z = data_02157934[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_0215775c[3];
  {
    u16 x = data_02157934[9];
    u16 y = data_02157934[10];
    u16 z = data_02157934[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
