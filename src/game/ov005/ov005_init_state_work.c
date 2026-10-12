#include "types.h"
#include "game/unlocks.h"
#include "game/Unk_ov005_0236bba4.h"

void Mem_Copy32(const void* src, void* dst, u32 size);

extern const Unk_ov005_0236bba4_State data_ov005_0236a74c[41];

void Ov005_InitStateWork(Unk_ov005_0236bba4* work) {
  work->wall1Unlocked = GameFlag_IsSet(61);
  work->wall2Unlocked = GameFlag_IsSet(62);
  work->wall3Unlocked = GameFlag_IsSet(34);
  work->unk_44 = GameFlag_IsSet(64);
  work->unk_48 = GameFlag_IsSet(65);
  work->wallLocked = 0;
  work->prevWallLocked = 0;
  work->stateTable = work->states;
  Mem_Copy32(data_ov005_0236a74c, work->stateTable, sizeof(work->states));
}
