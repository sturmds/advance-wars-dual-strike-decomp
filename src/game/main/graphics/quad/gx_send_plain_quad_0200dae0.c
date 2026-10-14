#include "types.h"
#include "game/gx_quad.h"

#define GXFIFO_NORMAL ((volatile u32*)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32*)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32*)0x0400048c)

extern u32 data_021577ac[];
extern u16 data_02157844[QUAD_VERTEX_COORDS];

/* Sends a textured quad to the 3D geometry FIFO: four vertices, each
 * with a normal, a texture coordinate and a 16-bit position. */
void Gx_SendPlainQuad_0200dae0(void) {
  *GXFIFO_NORMAL = 0x127b6127;
  *GXFIFO_TEXCOORD = data_021577ac[1];
  {
    u16 x = data_02157844[0];
    u16 y = data_02157844[1];
    u16 z = data_02157844[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749d27;
  *GXFIFO_TEXCOORD = data_021577ac[0];
  {
    u16 x = data_02157844[3];
    u16 y = data_02157844[4];
    u16 z = data_02157844[5];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x12749ed8;
  *GXFIFO_TEXCOORD = data_021577ac[3];
  {
    u16 x = data_02157844[6];
    u16 y = data_02157844[7];
    u16 z = data_02157844[8];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }

  *GXFIFO_NORMAL = 0x127b62d8;
  *GXFIFO_TEXCOORD = data_021577ac[2];
  {
    u16 x = data_02157844[9];
    u16 y = data_02157844[10];
    u16 z = data_02157844[11];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
  }
}
