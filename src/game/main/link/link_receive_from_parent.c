#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c *data_0215fe8c;
extern Unk_0229cdfc data_0229cdfc[7];
extern Unk_0229cb4c data_0229cb4c[7];
extern u8 gLinkLastSequence[7];

void Mem_CopyBytes(const void *src, void *dest, u32 size);

/* Child side: queues a packet from the parent in peer 0's queues, flagged
 * packets (header bit 7) in data_0229cb4c, the rest in data_0229cdfc.
 * gLinkLastSequence[0] keeps the last sequence number (0x80 before the first);
 * a repeat of it is dropped. A full queue sets the error flag. */
void Link_ReceiveFromParent(LinkPacket *packet, u16 length)
{
    u8 next;

    if (packet == NULL) {
        return;
    }
    if (gLinkLastSequence[0] != 0x80 && gLinkLastSequence[0] == (packet->header & 0x7f)) {
        return;
    }
    gLinkLastSequence[0] = packet->header & 0x7f;
    if (packet->header & 0x80) {
        next = data_0229cb4c[0].writeIndex + 1;
        if (next == 4) {
            next = 0;
        }
        if (next == data_0229cb4c[0].readIndex) {
            data_0215fe8c->error = 1;
            return;
        }
        Mem_CopyBytes(packet, &data_0229cb4c[0].slots[data_0229cb4c[0].writeIndex], sizeof(LinkPacket));
        data_0229cb4c[0].writeIndex = next;
    } else {
        next = data_0229cdfc[0].writeIndex + 1;
        if (next == 16) {
            next = 0;
        }
        if (next == data_0229cdfc[0].readIndex) {
            data_0215fe8c->error = 1;
            return;
        }
        Mem_CopyBytes(packet, &data_0229cdfc[0].slots[data_0229cdfc[0].writeIndex], sizeof(LinkPacket));
        data_0229cdfc[0].writeIndex = next;
    }
}
