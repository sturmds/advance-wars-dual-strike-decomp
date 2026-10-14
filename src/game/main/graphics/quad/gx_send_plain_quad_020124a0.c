#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)
static u32 data_0215780c[] = {
#include "data/main/data_0215780c.inc.c"
};
u32 data_0215781c[] = {
#include "data/main/data_0215781c.inc.c"
};
u16 data_0215782c[QUAD_VERTEX_COORDS] = {
#include "data/main/data_0215782c.inc.c"
};
u16 data_02157844[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157844.inc.c"
};
extern u16 data_02157a6c[QUAD_VERTEX_COORDS];

void Gx_SendPlainQuad_020124a0(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_0215780c[0];
  {
    u16 x = data_02157a6c[0];
    u16 y = data_02157a6c[1];
    u16 z = data_02157a6c[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_0215780c[1];
  {
    u16 x = data_02157a6c[3];
    u16 y = data_02157a6c[4];
    u16 z = data_02157a6c[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_0215780c[2];
  {
    u16 x = data_02157a6c[6];
    u16 y = data_02157a6c[7];
    u16 z = data_02157a6c[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_0215780c[3];
  {
    u16 x = data_02157a6c[9];
    u16 y = data_02157a6c[10];
    u16 z = data_02157a6c[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
