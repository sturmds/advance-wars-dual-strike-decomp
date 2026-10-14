#include "types.h"
#include "game/Unk_02088d04.h"

void TransferQueue_Add(const void* src, void* dst, u32 size);
extern u16 gEventTextMap[];
extern Unk_027c0280* data_027c0280;

void EventTextMap_QueueCopy(void) {
  TransferQueue_Add(gEventTextMap, data_027c0280->bgVram + 0x1f000, 0x180);
}
