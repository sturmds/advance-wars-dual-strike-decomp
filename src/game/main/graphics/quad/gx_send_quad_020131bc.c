#include "types.h"
#include "game/gx_quad.h"

extern u32 data_021577fc[];
u16 data_021578ec[QUAD_VERTEX_COORDS] = {
#include "data/main/data_021578ec.inc.c"
};
extern u16 data_022a7c64[];

void Gx_SendQuad_020131bc(void) {
  *((u32*)0x4000484) = 0x127b6127;
  *((u32*)0x4000488) = data_021577fc[0];
  *((u32*)0x4000480) = data_022a7c64[0];
  {
    u32 x = data_021578ec[0];
    u32 y = data_021578ec[1];
    u32 z = data_021578ec[2];
    *((u32*)0x400048c) = x | (y << 16);
    *((u32*)0x400048c) = z;
  }
  *((u32*)0x4000484) = 0x12749d27;
  *((u32*)0x4000488) = data_021577fc[1];
  *((u32*)0x4000480) = data_022a7c64[1];
  {
    u32 x = data_021578ec[3];
    u32 y = data_021578ec[4];
    u32 z = data_021578ec[5];
    *((u32*)0x400048c) = x | (y << 16);
    *((u32*)0x400048c) = z;
  }
  *((u32*)0x4000484) = 0x12749ed8;
  *((u32*)0x4000488) = data_021577fc[2];
  *((u32*)0x4000480) = data_022a7c64[2];
  {
    u32 x = data_021578ec[6];
    u32 y = data_021578ec[7];
    u32 z = data_021578ec[8];
    *((u32*)0x400048c) = x | (y << 16);
    *((u32*)0x400048c) = z;
  }
  *((u32*)0x4000484) = 0x127b62d8;
  *((u32*)0x4000488) = data_021577fc[3];
  *((u32*)0x4000480) = data_022a7c64[3];
  {
    u32 x = data_021578ec[9];
    u32 y = data_021578ec[10];
    u32 z = data_021578ec[11];
    *((u32*)0x400048c) = x | (y << 16);
    *((u32*)0x400048c) = z;
  }
}
