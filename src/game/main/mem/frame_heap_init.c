#include "types.h"
#include "game/heap.h"

#define FRAME_HEAP_SIGNATURE 0x46524D48 /* "FRMH" */

/* Sets up a frame heap in [start, end): the header, then one free region
 * that covers the rest. */
FrameHeap* FrameHeap_Init(void* start, void* end, u16 options) {
  FrameHeap* heap = start;
  FrameHeapData* data = &heap->data;

  Heap_InitHead(&heap->head, FRAME_HEAP_SIGNATURE, data + 1, end, options);
  data->free.start = heap->head.start;
  data->free.end = heap->head.end;
  data->states = NULL;
  return heap;
}
