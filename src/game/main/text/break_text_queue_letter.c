#include "types.h"
#include "game/break_text.h"

extern u8 data_027c0000[];
void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

/* Queues letter index of the break text on sprite list `list`. x and y carry
 * OAM attribute bits above the position; when bit 9 of y is set the sprite
 * is moved 8 pixels left and 16 up first. */
void BreakText_QueueLetter(u8 index, u16 x, u16 y, int attr, u8 list) {
  if (y & 0x200) {
    x = ((x & 0x1ff) - 8) & 0x1ff | (x & 0xfe00);
    y = ((y & 0xff) - 16) & 0xff | (y & 0xff00);
  }
  Sprite_Queue(list, x, y, data_027c0000, attr | gBreakText->palette << 12,
               gBreakText->tile + index * 8);
}
