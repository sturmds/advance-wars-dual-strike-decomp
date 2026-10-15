#include "types.h"
#include "game/event_script.h"
#include "game/Unk_02088d04.h"

void TransferQueue_Add(const void* src, void* dst, u32 size);

void EventTextMap_QueueCopy(void) {
  TransferQueue_Add(gEventTextMap[0], data_027c0280->bgVram + 0x1f000, 0x180);
}
