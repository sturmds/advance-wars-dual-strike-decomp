#include "types.h"
#include "game/event_script.h"

typedef struct Unk_020d80c4 Unk_020d80c4;

struct Unk_020d80c4 {
  u8 pad_00[0x64];
  s16 unk_64;
};

extern EventScriptCommand data_021677f0[];

void func_020d80c4(Unk_020d80c4* a0) {
  EventScriptSlot* t0 = EventScript_Start(data_021677f0);
  t0->unk_10 = a0->unk_64;
}
