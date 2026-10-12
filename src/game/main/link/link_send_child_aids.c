#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c* data_0215fe8c;

int Link_QueuePacket(LinkPacket* packet);
u16 Wireless_GetChildAids(void);

/* Sends packet type 2 carrying the 16-bit value of Wireless_GetChildAids, low byte first. */
void Link_SendChildAids(void) {
  LinkPacket packet;

  packet.header = data_0215fe8c->unk_0e & 0x7f;
  packet.type = 2;
  packet.header |= 0x80;
  packet.payload[0] = Wireless_GetChildAids();
  packet.payload[1] = Wireless_GetChildAids() >> 8;
  data_0215fe8c->unk_0e++;
  Link_QueuePacket(&packet);
}
