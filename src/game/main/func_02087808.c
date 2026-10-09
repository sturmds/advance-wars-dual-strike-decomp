#include "types.h"
#include "game/event_queue.h"

typedef struct Unk_02087808_0 Unk_02087808_0;

struct Unk_02087808_0 {
    u8 pad_00[0x7e];
    u8 unk_7e;
};

extern Unk_02087808_0 *data_02160ac0;

void func_02087808(void)
{
    EventQueue_Push(1, data_02160ac0->unk_7e);
}
