#include "types.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577dc[4];
extern u16 data_02157a24[12];

void Gx_SendPlainQuad_0200e068(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577dc[1];
  {
    u16 x = data_02157a24[0];
    u16 y = data_02157a24[1];
    u16 z = data_02157a24[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577dc[0];
  {
    u16 x = data_02157a24[3];
    u16 y = data_02157a24[4];
    u16 z = data_02157a24[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577dc[3];
  {
    u16 x = data_02157a24[6];
    u16 y = data_02157a24[7];
    u16 z = data_02157a24[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577dc[2];
  {
    u16 x = data_02157a24[9];
    u16 y = data_02157a24[10];
    u16 z = data_02157a24[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
