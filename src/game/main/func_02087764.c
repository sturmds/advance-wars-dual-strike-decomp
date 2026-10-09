#include "types.h"

typedef struct Unk_02087764_0 Unk_02087764_0;

struct Unk_02087764_0 {
    u8 pad_00[0x7e];
    u8 unk_7e;
};

void Sound_PlaySe(int);
extern Unk_02087764_0 *data_02160ac0;

void func_02087764(int a0)
{
    Sound_PlaySe(0x10002);
    data_02160ac0->unk_7e = a0;
}
