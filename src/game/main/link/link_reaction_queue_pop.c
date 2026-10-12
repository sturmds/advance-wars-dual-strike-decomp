#include "types.h"
#include "game/link_reaction_queue.h"

void Mem_CopyBytes(void* src, void* dest, int size);

/* Copies the oldest queued reaction to dest. Returns 0 when the queue is empty. */
int LinkReactionQueue_Pop(LinkReaction* dest) {
  if (data_0229da68.readIndex == data_0229da68.writeIndex) {
    return 0;
  }
  Mem_CopyBytes(&data_0229da68.slots[data_0229da68.readIndex], dest,
                sizeof(LinkReaction));
  data_0229da68.readIndex++;
  if (data_0229da68.readIndex == 16) {
    data_0229da68.readIndex = 0;
  }
  return 1;
}
