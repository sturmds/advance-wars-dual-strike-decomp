#include "types.h"

typedef struct Unk_0201d7d0_0 Unk_0201d7d0_0;

struct Unk_0201d7d0_0 {
  u8 pad_00[0x130];
  int unk_130;
};

void TransferQueue_Add(void*, int, int);
extern u16 gEventTextMap[];
extern Unk_0201d7d0_0* data_027c0280;

void EventTextMap_QueueCopy(void) {
  TransferQueue_Add(gEventTextMap, data_027c0280->unk_130 + 0x1f000, 0x180);
}
