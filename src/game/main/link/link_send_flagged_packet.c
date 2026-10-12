#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c* data_0215fe8c;

int Link_QueuePacket(LinkPacket* packet);
void Mem_CopyUnits(void* src, void* dest, int size, int unitSize);

/* Sends a copy of a 24-byte packet with a new sequence number and bit 7 set in the header. */
void Link_SendFlaggedPacket(void* data) {
  LinkPacket packet;

  Mem_CopyUnits(data, &packet, 24, 8);
  packet.header = data_0215fe8c->unk_0e & 0x7f;
  packet.header |= 0x80;
  data_0215fe8c->unk_0e++;
  Link_QueuePacket(&packet);
}
