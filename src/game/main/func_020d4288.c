#include "types.h"
#include "game/map_data.h"

typedef struct Unk_020d4288 Unk_020d4288;

struct Unk_020d4288 {
  u8 pad_00[0x50];
  int unk_50;
};

int Mem_Alloc(int);
int Map_SetDataBuffer(int);

int func_020d4288(Unk_020d4288* a0) {
  int t0 = Mem_Alloc(sizeof(MapDataBuffer));
  a0->unk_50 = t0;
  return Map_SetDataBuffer(a0->unk_50);
}
