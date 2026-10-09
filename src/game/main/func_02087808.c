#include "types.h"

typedef struct Unk_02087808_0 Unk_02087808_0;

struct Unk_02087808_0 {
    u8 pad_00[0x7e];
    u8 unk_7e;
};

void EventQueue_Push(int, int);
extern Unk_02087808_0 *data_02160ac0;

void func_02087808(void)
{
    EventQueue_Push(1, data_02160ac0->unk_7e);
}
