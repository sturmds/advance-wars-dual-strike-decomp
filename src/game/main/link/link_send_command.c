#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c* data_0215fe8c;

int Link_QueuePacket(LinkPacket* packet);

/* Sends a packet that holds only a type byte. */
void Link_SendCommand(int type) {
  LinkPacket packet;

  packet.header = data_0215fe8c->unk_0e & 0x7f;
  packet.type = type;
  packet.header |= 0x80;
  data_0215fe8c->unk_0e++;
  Link_QueuePacket(&packet);
}
