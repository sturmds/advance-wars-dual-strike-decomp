#include "types.h"

typedef struct Unk_0208c1f4_0 Unk_0208c1f4_0;

struct Unk_0208c1f4_0 {
  u8 pad_00[0x1];
  u8 unk_1;
};

extern Unk_0208c1f4_0* gAiTurnState;

void func_0208c1f4(int a0) { gAiTurnState->unk_1 = a0; }
