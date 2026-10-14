#include "types.h"
#include "game/gx_quad.h"

extern u32 data_0215776c[];
u16 data_02157934[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157934.inc.c"
};
u16 data_0215791c[QUAD_VERTEX_COORDS] = {
#include "data/main/data_0215791c.inc.c"
};
u16 data_02157904[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157904.inc.c"
};
u16 data_02157acc[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157acc.inc.c"
};
u16 data_02157ab4[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157ab4.inc.c"
};
u16 data_02157a9c[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157a9c.inc.c"
};
u16 data_02157a84[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157a84.inc.c"
};
u16 data_02157a6c[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157a6c.inc.c"
};
u16 data_02157a54[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157a54.inc.c"
};
u16 data_02157a3c[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157a3c.inc.c"
};
u16 data_02157a24[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157a24.inc.c"
};
u16 data_02157a0c[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157a0c.inc.c"
};
u16 data_021579f4[QUAD_VERTEX_COORDS] = {
#include "data/main/data_021579f4.inc.c"
};
u16 data_021579dc[QUAD_VERTEX_COORDS] = {
#include "data/main/data_021579dc.inc.c"
};
u16 data_021579c4[QUAD_VERTEX_COORDS] = {
#include "data/main/data_021579c4.inc.c"
};
u16 data_021579ac[QUAD_VERTEX_COORDS] = {
#include "data/main/data_021579ac.inc.c"
};
u16 data_02157994[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157994.inc.c"
};
u16 data_0215797c[QUAD_VERTEX_COORDS] = {
#include "data/main/data_0215797c.inc.c"
};
u16 data_02157964[QUAD_VERTEX_COORDS] = {
#include "data/main/data_02157964.inc.c"
};
u16 data_0215794c[QUAD_VERTEX_COORDS] = {
#include "data/main/data_0215794c.inc.c"
};
/* texture palette per terrain id */
u16 data_02157ae4[] = {
#include "data/main/data_02157ae4.inc.c"
};
/* per tall tile graphic: [0] TILE_DRAW_* */
u8 data_02157b24[][2] = {
#include "data/main/data_02157b24.inc.c"
};
/* per map cell code: tile model, with flags 0x4000 and 0x8000 */
u16 data_02157bc4[] = {
#include "data/main/data_02157bc4.inc.c"
};
/* the same for map 248 */
u16 data_02157f84[] = {
#include "data/main/data_02157f84.inc.c"
};
extern u16 data_022a7c64[];

void Gx_SendQuad_020135cc(void) {
  *((u32*)0x4000484) = 0x127b6127;
  *((u32*)0x4000488) = data_0215776c[0];
  *((u32*)0x4000480) = data_022a7c64[0];
  {
    u32 x = data_02157904[0];
    u32 y = data_02157904[1];
    u32 z = data_02157904[2];
    *((u32*)0x400048c) = x | (y << 16);
    *((u32*)0x400048c) = z;
  }
  *((u32*)0x4000484) = 0x12749d27;
  *((u32*)0x4000488) = data_0215776c[1];
  *((u32*)0x4000480) = data_022a7c64[1];
  {
    u32 x = data_02157904[3];
    u32 y = data_02157904[4];
    u32 z = data_02157904[5];
    *((u32*)0x400048c) = x | (y << 16);
    *((u32*)0x400048c) = z;
  }
  *((u32*)0x4000484) = 0x12749ed8;
  *((u32*)0x4000488) = data_0215776c[2];
  *((u32*)0x4000480) = data_022a7c64[2];
  {
    u32 x = data_02157904[6];
    u32 y = data_02157904[7];
    u32 z = data_02157904[8];
    *((u32*)0x400048c) = x | (y << 16);
    *((u32*)0x400048c) = z;
  }
  *((u32*)0x4000484) = 0x127b62d8;
  *((u32*)0x4000488) = data_0215776c[3];
  *((u32*)0x4000480) = data_022a7c64[3];
  {
    u32 x = data_02157904[9];
    u32 y = data_02157904[10];
    u32 z = data_02157904[11];
    *((u32*)0x400048c) = x | (y << 16);
    *((u32*)0x400048c) = z;
  }
}
