#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c* data_0215fe8c;

int Link_QueuePacket(LinkPacket* packet);
void Mem_CopyUnits(void* src, void* dest, int size, int unitSize);
int func_0207a260(void);
void LinkReactionQueue_Push(void* data);

/* Sends a 2-byte payload as packet type 18. When func_0207a260 returns 0, also passes it to
 * LinkReactionQueue_Push (presumably so the sender sees its own message). */
void LinkReaction_Send(void* data) {
  LinkPacket packet;

  Mem_CopyUnits(data, packet.payload, LINK_REACTION_SIZE, 8);
  packet.header = data_0215fe8c->unk_0e & 0x7f;
  packet.header |= 0x80;
  packet.type = LINK_PACKET_REACTION;
  data_0215fe8c->unk_0e++;
  Link_QueuePacket(&packet);
  if (func_0207a260() != 0) {
    return;
  }
  LinkReactionQueue_Push(data);
}
