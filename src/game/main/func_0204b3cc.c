#include "types.h"
#include "game/task.h"

typedef struct Unk_0204b3cc_0 Unk_0204b3cc_0;
typedef struct Unk_0204b3cc Unk_0204b3cc;

struct Unk_0204b3cc_0 {
  u8 pad_00[0x4c];
  int unk_4c;
};

struct Unk_0204b3cc {
  u8 pad_00[0x4c];
  int unk_4c;
};

extern const TaskScriptCmd data_0215afdc[];

void func_0204b3cc(Unk_0204b3cc* a0) {
  Unk_0204b3cc_0* t0 = (Unk_0204b3cc_0*)Task_Create(data_0215afdc, a0);
  t0->unk_4c = a0->unk_4c;
}
