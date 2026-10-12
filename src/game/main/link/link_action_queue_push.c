#include "types.h"
#include "game/Unk_0229d8e0.h"

void Mem_CopyBytes(void* src, void* dest, int size);
void Link_FlagError(void);

/* Queues a copy of the action. A full queue flags a link error and returns 0. */
int LinkActionQueue_Push(UnitAction* action) {
  u8 writeIndex = data_0229d8e0.writeIndex;
  u8 next = writeIndex + 1;

  if (next == 16) {
    next = 0;
  }
  if (next == data_0229d8e0.readIndex) {
    Link_FlagError();
    return 0;
  }
  Mem_CopyBytes(action, &data_0229d8e0.slots[writeIndex], sizeof(UnitAction));
  data_0229d8e0.writeIndex = next;
  return 1;
}
