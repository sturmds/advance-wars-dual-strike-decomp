#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577dc[];
extern u16 data_02157874[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_02010ad0(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577dc[1];
  {
    u16 x = data_02157874[0];
    u16 y = data_02157874[1];
    u16 z = data_02157874[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577dc[0];
  {
    u16 x = data_02157874[3];
    u16 y = data_02157874[4];
    u16 z = data_02157874[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577dc[3];
  {
    u16 x = data_02157874[6];
    u16 y = data_02157874[7];
    u16 z = data_02157874[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577dc[2];
  {
    u16 x = data_02157874[9];
    u16 y = data_02157874[10];
    u16 z = data_02157874[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
