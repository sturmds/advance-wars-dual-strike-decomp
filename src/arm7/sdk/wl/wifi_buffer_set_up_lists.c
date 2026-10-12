#include "sdk/wl.h"

void func_arm7_027c7f14(WlBufferList* list,
                        int id); /* empties the list and sets its id */

/* Copies the buffer allocator, sets up the buffer lists (ids 2 to 12) and takes the data block of unk_318. */
void WifiBuffer_SetUpLists(const WlBufferAllocator* allocator) {
  WlWork* work = WIFI_DRIVER_STATE;
  WlBufferPool* pool = (WlBufferPool*)&work->allocator;
  WlUnk318* unk = &work->unk_318;

  pool->allocator.kind = allocator->kind;
  pool->allocator.a = allocator->a;
  pool->allocator.b = allocator->b;
  func_arm7_027c7f14(&pool->newBuffers, 2);
  func_arm7_027c7f14(&pool->lists[0], 3);
  func_arm7_027c7f14(&pool->lists[1], 4);
  func_arm7_027c7f14(&pool->lists[2], 5);
  func_arm7_027c7f14(&pool->lists[3], 6);
  func_arm7_027c7f14(&pool->lists[4], 7);
  func_arm7_027c7f14(&pool->lists[5], 8);
  func_arm7_027c7f14(&pool->lists[6], 9);
  func_arm7_027c7f14(&pool->lists[7], 10);
  func_arm7_027c7f14(&pool->outbox, 11);
  func_arm7_027c7f14(&pool->inbox, 12);
  /* + 1: the data starts after the buffer header */
  unk->data = (u8*)(WifiBuffer_New(&pool->newBuffers, 0x81) + 1);
  unk->dataLength = 0;
  unk->unk_a4 = 0;
}
