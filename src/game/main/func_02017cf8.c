#include "types.h"
#include "game/script_slot.h"

typedef struct Unk_02017cf8_0 Unk_02017cf8_0;

struct Unk_02017cf8_0 {
  u8 pad_00[0x1e];
  u16 unk_1e;
};

Unk_02017cf8_0* ScriptSlot_Start(void*, int);
extern ScriptCommand data_02158468[];

void func_02017cf8(int a0) {
  Unk_02017cf8_0* t0 = ScriptSlot_Start(data_02158468, 0);
  t0->unk_1e = a0;
}
