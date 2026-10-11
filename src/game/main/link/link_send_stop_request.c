#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c *data_0215fe8c;

u32 func_0207bd80(void);
int func_0207a260(void);
void Link_SendCommand(int type);

/* In link mode, clears unk_0c and sends a stop packet when func_0207a260 is nonzero. */
void Link_SendStopRequest(void)
{
    if (func_0207bd80() == 0) {
        return;
    }
    data_0215fe8c->unk_0c = 0;
    if (func_0207a260() == 0) {
        return;
    }
    Link_SendCommand(LINK_PACKET_STOP);
}
