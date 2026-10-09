#include "types.h"

typedef struct Unk_02364428 Unk_02364428;

struct Unk_02364428 {
    u8 pad_00[0x50];
    int unk_50;
};

int Task_Create(void *, int);
extern u8 data_ov005_0236aef4[];

int func_ov005_02364428(Unk_02364428 *a0)
{
    int t0 = Task_Create(data_ov005_0236aef4, 3);
    a0->unk_50 = t0;
    return t0;
}
