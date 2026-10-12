#include "types.h"

typedef struct Unk_020995a4_0 Unk_020995a4_0;

struct Unk_020995a4_0 {
  u8 pad_00[0x2];
  u8 unk_2;
};

extern Unk_020995a4_0* gAiTurnState;

int AiTurn_IsPhase3(void) { return (gAiTurnState->unk_2 == 3) ? 1 : 0; }
