#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
extern u32 data_021577ec[];
extern u16 data_02157acc[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_0201258c(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577ec[0];
  {
    u16 x = data_02157acc[0];
    u16 y = data_02157acc[1];
    u16 z = data_02157acc[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577ec[1];
  {
    u16 x = data_02157acc[3];
    u16 y = data_02157acc[4];
    u16 z = data_02157acc[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577ec[2];
  {
    u16 x = data_02157acc[6];
    u16 y = data_02157acc[7];
    u16 z = data_02157acc[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577ec[3];
  {
    u16 x = data_02157acc[9];
    u16 y = data_02157acc[10];
    u16 z = data_02157acc[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
