#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577ec[];
extern u16 data_02157a54[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_02011990(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577ec[3];
  {
    u16 x = data_02157a54[0];
    u16 y = data_02157a54[1];
    u16 z = data_02157a54[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577ec[2];
  {
    u16 x = data_02157a54[3];
    u16 y = data_02157a54[4];
    u16 z = data_02157a54[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577ec[1];
  {
    u16 x = data_02157a54[6];
    u16 y = data_02157a54[7];
    u16 z = data_02157a54[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577ec[0];
  {
    u16 x = data_02157a54[9];
    u16 y = data_02157a54[10];
    u16 z = data_02157a54[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
