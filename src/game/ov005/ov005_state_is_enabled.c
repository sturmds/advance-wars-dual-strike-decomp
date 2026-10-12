#include "types.h"
#include "game/Unk_ov005_0236bba4.h"

BOOL Ov005_StateIsEnabled(Unk_ov005_0236bba4* work) {
  return work->stateTable[work->state].unk_00 != 0;
}
