#include "types.h"
#include "game/Unk_0215fe8c.h"
#include "game/link_buffers.h"

extern Unk_0215fe8c* data_0215fe8c;
extern Unk_0229ca88 data_0229ca88;

void Mem_CopyBytes(void* src, void* dest, int size);

/* Puts a packet on the send queue. A full queue flags a link error and returns 0. */
int Link_QueuePacket(LinkPacket* packet) {
  u8 writeIndex = data_0229ca88.writeIndex;
  u8 next = writeIndex + 1;

  if (next == LINK_SEND_QUEUE_SLOTS) {
    next = 0;
  }
  if (next == data_0229ca88.readIndex) {
    data_0215fe8c->error = 1;
    return 0;
  }
  Mem_CopyBytes(packet, &data_0229ca88.slots[writeIndex], sizeof(LinkPacket));
  data_0229ca88.writeIndex = next;
  return 1;
}
