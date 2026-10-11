#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c *data_0215fe8c;

void Link_SendQueuedPacket(void);

/* In link mode, sends the next queued packet. */
void Link_FlushSendQueue(void)
{
    if (data_0215fe8c->linkActive == 0) {
        return;
    }
    Link_SendQueuedPacket();
}
