#include "types.h"

typedef struct Unk_020eee70 Unk_020eee70;

struct Unk_020eee70 {
    u8 pad_00[0x64];
    s16 unk_64;
    s16 unk_66;
};

void func_020ed4b0(int, int, void *);

void func_020eee70(Unk_020eee70 *a0)
{
    func_020ed4b0(a0->unk_64, a0->unk_66, a0);
}
