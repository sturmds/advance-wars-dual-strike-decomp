#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

extern u8 data_027c0048;

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

#define STATE data_ov003_02364928

/* Queues one sprite for a slot: the column comes from the high nibble of its
 * row's byte 0x3b, the line from Ov003Slot.cursor (10 and up counts from 0 again,
 * 3 columns over). With data_02160ac0->unk_7c set the attribute word gets 16 more
 * per slot. What it shows is not known. */
void Ov003_DrawSlotMarker(int slot) {
  int y = STATE->slots[slot].cursor;
  int column = STATE->rows[STATE->slots[slot].row - 1].unk_3b_4;
  int x = column * 4;

  if (y >= 10) {
    y -= 10;
    x += 3;
  }
  x = x * 8 + 4;
  if (data_02160ac0->unk_7c == 0) {
    Sprite_Queue(0, x & 0x1ff, (y * 16) & 0xff, &data_027c0048, 44, 0);
  } else {
    Sprite_Queue(0, x & 0x1ff, (y * 16) & 0xff, &data_027c0048,
                 44 + (slot + 1) * 16, 0);
  }
}
