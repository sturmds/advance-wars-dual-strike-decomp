#include "types.h"

#define GXFIFO_NORMAL ((volatile u32 *)0x04000484)
#define GXFIFO_TEXCOORD ((volatile u32 *)0x04000488)
#define GXFIFO_VTX_16 ((volatile u32 *)0x0400048c)

extern u32 data_021577ac[4];
extern u16 data_02157844[12];

static inline void SendVertex(const u16 *v)
{
    u16 x = v[0];
    u16 y = v[1];
    u16 z = v[2];
    *GXFIFO_VTX_16 = x | (y << 16);
    *GXFIFO_VTX_16 = z;
}

/* Sends a textured quad to the 3D geometry FIFO: four vertices, each
 * with a normal, a texture coordinate and a 16-bit position. */
void func_0200cfd0(void)
{
    *GXFIFO_NORMAL = 0x127b6127;
    *GXFIFO_TEXCOORD = data_021577ac[2];
    SendVertex(&data_02157844[0]);

    *GXFIFO_NORMAL = 0x12749d27;
    *GXFIFO_TEXCOORD = data_021577ac[3];
    SendVertex(&data_02157844[3]);

    *GXFIFO_NORMAL = 0x12749ed8;
    *GXFIFO_TEXCOORD = data_021577ac[0];
    SendVertex(&data_02157844[6]);

    *GXFIFO_NORMAL = 0x127b62d8;
    *GXFIFO_TEXCOORD = data_021577ac[1];
    SendVertex(&data_02157844[9]);
}
