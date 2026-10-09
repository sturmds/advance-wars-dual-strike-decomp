#include "types.h"

typedef struct Unk_023622b8_0 Unk_023622b8_0;

struct Unk_023622b8_0 {
    u8 pad_00[0x7c];
    u8 unk_7c;
};

extern Unk_023622b8_0 *data_02160ac0;

int func_ov003_023622b8(void)
{
    return (data_02160ac0->unk_7c != 0) ? 8 : 1;
}
