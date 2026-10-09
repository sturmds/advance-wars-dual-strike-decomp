#include "types.h"

typedef struct Unk_020fccbc Unk_020fccbc;

struct Unk_020fccbc {
    u8 pad_00[0x30];
    int unk_30;
};

int func_020fccbc(Unk_020fccbc *a0)
{
    return a0->unk_30;
}
