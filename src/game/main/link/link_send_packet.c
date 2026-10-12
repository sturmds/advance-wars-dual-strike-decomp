#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c* data_0215fe8c;

void Mem_CopyUnits(void* src, void* dest, int size, int unitSize);
int func_0207a260(void);
int Link_QueuePacket(LinkPacket* packet);

void Link_SendPacket(void* payload) {
  LinkPacket packet;

  Mem_CopyUnits(payload, packet.payload, 24, 8);
  packet.header = data_0215fe8c->unk_0e & 0x7f;
  packet.header |= 0x80;
  packet.type = func_0207a260() + 14;
  data_0215fe8c->unk_0e++;
  Link_QueuePacket(&packet);
}
