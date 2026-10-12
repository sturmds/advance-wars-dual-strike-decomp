#include "types.h"
#include "game/link_reaction_queue.h"

void Mem_CopyBytes(void* src, void* dest, int size);

/* Queues a copy of the reaction. Returns 0 when the queue is full. */
int LinkReactionQueue_Push(LinkReaction* reaction) {
  u8 writeIndex = data_0229da68.writeIndex;
  u8 next = writeIndex + 1;

  if (next == 16) {
    next = 0;
  }
  if (next == data_0229da68.readIndex) {
    return 0;
  }
  Mem_CopyBytes(reaction, &data_0229da68.slots[writeIndex],
                sizeof(LinkReaction));
  data_0229da68.writeIndex = next;
  return 1;
}
