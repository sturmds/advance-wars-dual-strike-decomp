#include "types.h"

typedef struct Unk_021062e8_0 Unk_021062e8_0;
typedef struct Unk_021062e8_1 Unk_021062e8_1;

struct Unk_021062e8_0 {
    u8 pad_00[0x14];
    int unk_14;
};

struct Unk_021062e8_1 {
    u8 pad_00[0x80];
    int unk_80;
};

extern Unk_021062e8_1 *data_022a8a60;

void func_021062e8(int a0, int a1)
{
    ((Unk_021062e8_0 *)(data_022a8a60->unk_80 + (a0 << 4)))->unk_14 = a1;
}
