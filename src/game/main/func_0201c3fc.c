#include "types.h"

typedef struct Unk_0201c3fc Unk_0201c3fc;

struct Unk_0201c3fc {
    u8 pad_00[0x8];
    int unk_8;
};

int Task_FindByScript(void *);
extern u8 data_02158548[];

int func_0201c3fc(Unk_0201c3fc *a0)
{
    int t0 = Task_FindByScript(data_02158548);
    if (t0 == 0) {
        a0->unk_8 = 0;
        return 0;
    }
    return t0;
}
