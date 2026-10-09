#include "types.h"
#include "game/Unk_0215fe8c.h"

typedef struct LinkPacket {
    u8 header;                         /* sequence number in the low 7 bits, bit 7 set */
    u8 type;
    u8 payload[24];
} LinkPacket;

extern Unk_0215fe8c *data_0215fe8c;

void Mem_CopyUnits(void *src, void *dest, int size, int unitSize);
int func_0207a260(void);
void func_0207b978(LinkPacket *packet);

void Link_SendPacket(void *payload)
{
    LinkPacket packet;

    Mem_CopyUnits(payload, packet.payload, 24, 8);
    packet.header = data_0215fe8c->unk_0e & 0x7f;
    packet.header |= 0x80;
    packet.type = func_0207a260() + 14;
    data_0215fe8c->unk_0e++;
    func_0207b978(&packet);
}
