#include "types.h"
#include "game/Unk_02088d04.h"

int GameFlag_IsSet(int flag);
void TransferQueue_Add(const void* src, void* dst, u32 size);
u8* UnitType_GetPictureTiles(int unitType);

extern Unk_027c0280* data_027c0280;
s8 data_ov006_0236a7d0[] = {
#include "data/ov006/data_ov006_0236a7d0.inc.c"
};
s8 data_ov006_0236a7b4[] = {
#include "data/ov006/data_ov006_0236a7b4.inc.c"
};
/* (x, y) per entry when unitMode is 0 */
u8 data_ov006_0236a7ec[] = {
#include "data/ov006/data_ov006_0236a7ec.inc.c"
};

void func_ov006_02352400(void) {
  u8* dst;
  s8* table;
  int i;
  int count;

  dst = data_027c0280->objVram + 0x58c0;
  if (GameFlag_IsSet(0x2b) == 1) {
    table = data_ov006_0236a7d0;
    count = 26;
  } else {
    table = data_ov006_0236a7b4;
    count = 25;
  }
  for (i = 0; i < count; i++) {
    TransferQueue_Add(UnitType_GetPictureTiles(*table & 0x3f), dst, 256);
    dst += 256;
    table++;
  }
}
