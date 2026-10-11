#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c *data_0215fe8c;

int Link_QueuePacket(LinkPacket *packet);
u16 func_0207a1dc(void);

/* Sends packet type 2 carrying the 16-bit value of func_0207a1dc, low byte first. */
void func_0207b8b8(void)
{
    LinkPacket packet;

    packet.header = data_0215fe8c->unk_0e & 0x7f;
    packet.type = 2;
    packet.header |= 0x80;
    packet.payload[0] = func_0207a1dc();
    packet.payload[1] = func_0207a1dc() >> 8;
    data_0215fe8c->unk_0e++;
    Link_QueuePacket(&packet);
}
