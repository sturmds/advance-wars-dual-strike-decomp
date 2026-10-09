#include "types.h"

typedef struct Unk_0208be18_0 Unk_0208be18_0;

struct Unk_0208be18_0 {
    u8 pad_00[0x84];
    u8 unk_84;
};

extern Unk_0208be18_0 *data_02160ac0;

void func_0208be18(int a0)
{
    data_02160ac0->unk_84 = a0;
}
