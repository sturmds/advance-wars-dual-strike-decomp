#include "types.h"
#include "game/Unk_ov005_0236bba4.h"

void Ov005_RunState(Unk_ov005_0236bba4* work) {
  Unk_ov005_0236bba4_State* state = &work->stateTable[work->state];

  work->prevWallLocked = work->wallLocked;
  work->wallLocked = 0;
  state->run(work, state->arg);
}
