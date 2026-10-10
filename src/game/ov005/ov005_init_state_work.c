#include "types.h"
#include "game/Unk_ov005_0236bba4.h"

BOOL GameFlag_IsSet(int flag);
void Mem_Copy32(const void *src, void *dst, u32 size);

extern const Unk_ov005_0236bba4_State data_ov005_0236a74c[41];

void Ov005_InitStateWork(Unk_ov005_0236bba4 *work)
{
    work->unk_38 = GameFlag_IsSet(61);
    work->unk_3c = GameFlag_IsSet(62);
    work->unk_40 = GameFlag_IsSet(34);
    work->unk_44 = GameFlag_IsSet(64);
    work->unk_48 = GameFlag_IsSet(65);
    work->unk_4c = 0;
    work->unk_50 = 0;
    work->stateTable = work->states;
    Mem_Copy32(data_ov005_0236a74c, work->stateTable, sizeof(work->states));
}
