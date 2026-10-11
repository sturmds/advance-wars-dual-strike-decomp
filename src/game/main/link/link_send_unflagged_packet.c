#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c *data_0215fe8c;

int Link_QueuePacket(LinkPacket *packet);
void Mem_CopyBytes(void *src, void *dest, int size);

/* Sends unk_0f bytes after a header byte with the next sequence number and bit 7 clear. */
void Link_SendUnflaggedPacket(void *data)
{
    LinkPacket packet;

    packet.header = data_0215fe8c->unk_0e & 0x7f;
    Mem_CopyBytes(data, &packet.type, data_0215fe8c->packetSize);
    data_0215fe8c->unk_0e++;
    Link_QueuePacket(&packet);
}
