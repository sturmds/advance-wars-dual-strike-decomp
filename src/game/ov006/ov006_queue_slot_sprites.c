#include "types.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

typedef struct SlotSprite {
  u16 unk_00;
  u16 attr;
} SlotSprite;

typedef struct SlotSprites {
  SlotSprite slot[4];
} SlotSprites;

extern const SlotSprites data_ov006_0236a790;
extern void* data_ov006_0236a878[];

/* Queues four sprites in a column at x 23, y 16 and every 17 pixels after. Each
 * takes its image from data_ov006_0236a878 and its palette (the attr field,
 * shifted up 12 bits) from data_ov006_0236a790. */
void Ov006_QueueSlotSprites(void) {
  SlotSprites sprites;
  int i;
  int y;
  SlotSprite* slot;

  sprites = data_ov006_0236a790;
  slot = sprites.slot;
  y = 15;
  for (i = 0; i < 4; i++) {
    Sprite_Queue(0, 23, y + 1, data_ov006_0236a878[i], (slot++)->attr << 12, 0);
    y += 17;
  }
}
