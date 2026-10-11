#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c *data_0215fe8c;
extern u8 gLinkLastSequence[7];

/* The per-peer queues as Link_PopFlaggedPacket and Link_PopUnflaggedPacket
 * declare them: each index byte is a symbol of its own (cbac and cf7c read,
 * cbad and cf7d write). */
extern u8 data_0229cb4c[7][98];
extern u8 data_0229cbac[7][98];
extern u8 data_0229cbad[7][98];
extern u8 data_0229cdfc[7][386];
extern u8 data_0229cf7c[7][386];
extern u8 data_0229cf7d[7][386];

void Mem_CopyBytes(const void *src, void *dest, u32 size);

/* Parent side: queues a packet from child `aid` in peer aid - 1's queues, the
 * same way Link_ReceiveFromParent does for the child. */
void Link_ReceiveFromChild(u16 aid, LinkPacket *packet, u16 length)
{
    u8 next;

    if (packet == NULL) {
        return;
    }
    aid = aid - 1;
    if (gLinkLastSequence[aid] != 0x80 && gLinkLastSequence[aid] == (packet->header & 0x7f)) {
        return;
    }
    gLinkLastSequence[aid] = packet->header & 0x7f;
    if (packet->header & 0x80) {
        next = data_0229cbad[aid][0] + 1;
        if (next == 4) {
            next = 0;
        }
        if (next == data_0229cbac[aid][0]) {
            data_0215fe8c->error = 1;
            return;
        }
        Mem_CopyBytes(packet, &data_0229cb4c[aid][data_0229cbad[aid][0] * 24], sizeof(LinkPacket));
        data_0229cbad[aid][0] = next;
    } else {
        next = data_0229cf7d[aid][0] + 1;
        if (next == 16) {
            next = 0;
        }
        if (next == data_0229cf7c[aid][0]) {
            data_0215fe8c->error = 1;
            return;
        }
        Mem_CopyBytes(packet, &data_0229cdfc[aid][data_0229cf7d[aid][0] * 24], sizeof(LinkPacket));
        data_0229cf7d[aid][0] = next;
    }
}
