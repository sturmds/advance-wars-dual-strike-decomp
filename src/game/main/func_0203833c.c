#include "types.h"

typedef struct Unk_0203833c_0 Unk_0203833c_0;

struct Unk_0203833c_0 {
    u8 pad_00[0x9a];
    s16 unk_9a;
};

void func_020393dc(int, int, int);
extern Unk_0203833c_0 *data_0229c734;

void func_0203833c(void)
{
    func_020393dc(8, 160, data_0229c734->unk_9a);
}
