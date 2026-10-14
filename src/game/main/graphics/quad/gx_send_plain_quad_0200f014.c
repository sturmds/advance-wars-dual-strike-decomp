#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577ec[];
extern u16 data_02157964[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_0200f014(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577ec[3];
  {
    u16 x = data_02157964[0];
    u16 y = data_02157964[1];
    u16 z = data_02157964[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577ec[2];
  {
    u16 x = data_02157964[3];
    u16 y = data_02157964[4];
    u16 z = data_02157964[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577ec[1];
  {
    u16 x = data_02157964[6];
    u16 y = data_02157964[7];
    u16 z = data_02157964[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577ec[0];
  {
    u16 x = data_02157964[9];
    u16 y = data_02157964[10];
    u16 z = data_02157964[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
