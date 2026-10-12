#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

/* Stores a segment in the first empty slot. Halts when all slots are in use. */
void MbCache_Add(DlPlayHeldProgram* cache, u32 address, u32 size, u8* buffer,
                 u32 state) {
  OSIntrMode enabled = OS_DisableInterrupts();
  DlPlayHeldBlock* segment = cache->pages;
  DlPlayHeldBlock* end = cache->pages + DLPLAY_HELD_BLOCK_SLOTS;

  for (;; segment++) {
    if (segment >= end) {
      OS_Terminate();
    }
    if (segment->state == 0) {
      segment->address = address;
      segment->size = size;
      segment->buffer = buffer;
      segment->state = state;
      break;
    }
  }
  OS_RestoreInterrupts(enabled);
}
