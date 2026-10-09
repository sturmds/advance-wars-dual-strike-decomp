#include "types.h"

typedef struct Unk_020d0148 Unk_020d0148;

struct Unk_020d0148 {
    u8 pad_00[0x50];
    int unk_50;
};

void MenuDepth_Pop(void);
int func_0202244c(int);

int func_020d0148(Unk_020d0148 *a0)
{
    MenuDepth_Pop();
    return func_0202244c(a0->unk_50);
}
