#include "types.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

typedef struct SlotSprite {
  u16 unk_00;
  u16 attr;
} SlotSprite;

typedef struct SlotSprites {
  SlotSprite slot[4];
} SlotSprites;

u8 data_ov006_0236a4cc[4] = {
#include "data/ov006/data_ov006_0236a4cc.inc.c"
};
u32 data_ov006_0236a780[2] = {
#include "data/ov006/data_ov006_0236a780.inc.c"
};
u32 data_ov006_0236a778[2] = {
#include "data/ov006/data_ov006_0236a778.inc.c"
};
u32 data_ov006_0236a770[2] = {
#include "data/ov006/data_ov006_0236a770.inc.c"
};
u32 data_ov006_0236a768[2] = {
#include "data/ov006/data_ov006_0236a768.inc.c"
};
u32 data_ov006_0236a760[2] = {
#include "data/ov006/data_ov006_0236a760.inc.c"
};
u32 data_ov006_0236a758[2] = {
#include "data/ov006/data_ov006_0236a758.inc.c"
};
u32 data_ov006_0236a750[2] = {
#include "data/ov006/data_ov006_0236a750.inc.c"
};
u32 data_ov006_0236a748[2] = {
#include "data/ov006/data_ov006_0236a748.inc.c"
};
u32 data_ov006_0236a740[2] = {
#include "data/ov006/data_ov006_0236a740.inc.c"
};
u32 data_ov006_0236a738[2] = {
#include "data/ov006/data_ov006_0236a738.inc.c"
};
u32 data_ov006_0236a730[2] = {
#include "data/ov006/data_ov006_0236a730.inc.c"
};
u32 data_ov006_0236a728[2] = {
#include "data/ov006/data_ov006_0236a728.inc.c"
};
u32 data_ov006_0236a720[2] = {
#include "data/ov006/data_ov006_0236a720.inc.c"
};
u32 data_ov006_0236a718[2] = {
#include "data/ov006/data_ov006_0236a718.inc.c"
};
u32 data_ov006_0236a710[2] = {
#include "data/ov006/data_ov006_0236a710.inc.c"
};
u32 data_ov006_0236a708[2] = {
#include "data/ov006/data_ov006_0236a708.inc.c"
};
u32 data_ov006_0236a700[2] = {
#include "data/ov006/data_ov006_0236a700.inc.c"
};
u32 data_ov006_0236a6f8[2] = {
#include "data/ov006/data_ov006_0236a6f8.inc.c"
};
u32 data_ov006_0236a6f0[2] = {
#include "data/ov006/data_ov006_0236a6f0.inc.c"
};
u32 data_ov006_0236a6e8[2] = {
#include "data/ov006/data_ov006_0236a6e8.inc.c"
};
u32 data_ov006_0236a6e0[2] = {
#include "data/ov006/data_ov006_0236a6e0.inc.c"
};
u32 data_ov006_0236a6d8[2] = {
#include "data/ov006/data_ov006_0236a6d8.inc.c"
};
u32 data_ov006_0236a6d0[2] = {
#include "data/ov006/data_ov006_0236a6d0.inc.c"
};
u32 data_ov006_0236a6c8[2] = {
#include "data/ov006/data_ov006_0236a6c8.inc.c"
};
u32 data_ov006_0236a6c0[2] = {
#include "data/ov006/data_ov006_0236a6c0.inc.c"
};
u32 data_ov006_0236a4d8[2] = {
#include "data/ov006/data_ov006_0236a4d8.inc.c"
};
u32 data_ov006_0236a6b0[2] = {
#include "data/ov006/data_ov006_0236a6b0.inc.c"
};
u32 data_ov006_0236a6a8[2] = {
#include "data/ov006/data_ov006_0236a6a8.inc.c"
};
u32 data_ov006_0236a6a0[2] = {
#include "data/ov006/data_ov006_0236a6a0.inc.c"
};
u32 data_ov006_0236a698[2] = {
#include "data/ov006/data_ov006_0236a698.inc.c"
};
u32 data_ov006_0236a690[2] = {
#include "data/ov006/data_ov006_0236a690.inc.c"
};
u32 data_ov006_0236a688[2] = {
#include "data/ov006/data_ov006_0236a688.inc.c"
};
u32 data_ov006_0236a680[2] = {
#include "data/ov006/data_ov006_0236a680.inc.c"
};
u32 data_ov006_0236a678[2] = {
#include "data/ov006/data_ov006_0236a678.inc.c"
};
u32 data_ov006_0236a670[2] = {
#include "data/ov006/data_ov006_0236a670.inc.c"
};
u32 data_ov006_0236a668[2] = {
#include "data/ov006/data_ov006_0236a668.inc.c"
};
u32 data_ov006_0236a660[2] = {
#include "data/ov006/data_ov006_0236a660.inc.c"
};
u32 data_ov006_0236a658[2] = {
#include "data/ov006/data_ov006_0236a658.inc.c"
};
u32 data_ov006_0236a650[2] = {
#include "data/ov006/data_ov006_0236a650.inc.c"
};
u32 data_ov006_0236a648[2] = {
#include "data/ov006/data_ov006_0236a648.inc.c"
};
u32 data_ov006_0236a640[2] = {
#include "data/ov006/data_ov006_0236a640.inc.c"
};
u32 data_ov006_0236a638[2] = {
#include "data/ov006/data_ov006_0236a638.inc.c"
};
u32 data_ov006_0236a630[2] = {
#include "data/ov006/data_ov006_0236a630.inc.c"
};
u32 data_ov006_0236a628[2] = {
#include "data/ov006/data_ov006_0236a628.inc.c"
};
u32 data_ov006_0236a620[2] = {
#include "data/ov006/data_ov006_0236a620.inc.c"
};
u32 data_ov006_0236a618[2] = {
#include "data/ov006/data_ov006_0236a618.inc.c"
};
u32 data_ov006_0236a610[2] = {
#include "data/ov006/data_ov006_0236a610.inc.c"
};
u32 data_ov006_0236a608[2] = {
#include "data/ov006/data_ov006_0236a608.inc.c"
};
u32 data_ov006_0236a600[2] = {
#include "data/ov006/data_ov006_0236a600.inc.c"
};
u32 data_ov006_0236a5f8[2] = {
#include "data/ov006/data_ov006_0236a5f8.inc.c"
};
u32 data_ov006_0236a5f0[2] = {
#include "data/ov006/data_ov006_0236a5f0.inc.c"
};
u32 data_ov006_0236a5e8[2] = {
#include "data/ov006/data_ov006_0236a5e8.inc.c"
};
u32 data_ov006_0236a5e0[2] = {
#include "data/ov006/data_ov006_0236a5e0.inc.c"
};
u32 data_ov006_0236a5d8[2] = {
#include "data/ov006/data_ov006_0236a5d8.inc.c"
};
u32 data_ov006_0236a5d0[2] = {
#include "data/ov006/data_ov006_0236a5d0.inc.c"
};
u32 data_ov006_0236a5c8[2] = {
#include "data/ov006/data_ov006_0236a5c8.inc.c"
};
u32 data_ov006_0236a5c0[2] = {
#include "data/ov006/data_ov006_0236a5c0.inc.c"
};
u32 data_ov006_0236a4f8[2] = {
#include "data/ov006/data_ov006_0236a4f8.inc.c"
};
u32 data_ov006_0236a5b0[2] = {
#include "data/ov006/data_ov006_0236a5b0.inc.c"
};
u32 data_ov006_0236a5a8[2] = {
#include "data/ov006/data_ov006_0236a5a8.inc.c"
};
u32 data_ov006_0236a5a0[2] = {
#include "data/ov006/data_ov006_0236a5a0.inc.c"
};
u32 data_ov006_0236a598[2] = {
#include "data/ov006/data_ov006_0236a598.inc.c"
};
u32 data_ov006_0236a590[2] = {
#include "data/ov006/data_ov006_0236a590.inc.c"
};
u32 data_ov006_0236a588[2] = {
#include "data/ov006/data_ov006_0236a588.inc.c"
};
u32 data_ov006_0236a580[2] = {
#include "data/ov006/data_ov006_0236a580.inc.c"
};
u32 data_ov006_0236a578[2] = {
#include "data/ov006/data_ov006_0236a578.inc.c"
};
u32 data_ov006_0236a570[2] = {
#include "data/ov006/data_ov006_0236a570.inc.c"
};
u32 data_ov006_0236a568[2] = {
#include "data/ov006/data_ov006_0236a568.inc.c"
};
u32 data_ov006_0236a560[2] = {
#include "data/ov006/data_ov006_0236a560.inc.c"
};
u32 data_ov006_0236a558[2] = {
#include "data/ov006/data_ov006_0236a558.inc.c"
};
u32 data_ov006_0236a550[2] = {
#include "data/ov006/data_ov006_0236a550.inc.c"
};
u32 data_ov006_0236a548[2] = {
#include "data/ov006/data_ov006_0236a548.inc.c"
};
u32 data_ov006_0236a540[2] = {
#include "data/ov006/data_ov006_0236a540.inc.c"
};
u32 data_ov006_0236a538[2] = {
#include "data/ov006/data_ov006_0236a538.inc.c"
};
u32 data_ov006_0236a530[2] = {
#include "data/ov006/data_ov006_0236a530.inc.c"
};
u32 data_ov006_0236a528[2] = {
#include "data/ov006/data_ov006_0236a528.inc.c"
};
u32 data_ov006_0236a520[2] = {
#include "data/ov006/data_ov006_0236a520.inc.c"
};
u32 data_ov006_0236a518[2] = {
#include "data/ov006/data_ov006_0236a518.inc.c"
};
u32 data_ov006_0236a510[2] = {
#include "data/ov006/data_ov006_0236a510.inc.c"
};
u32 data_ov006_0236a508[2] = {
#include "data/ov006/data_ov006_0236a508.inc.c"
};
u32 data_ov006_0236a500[2] = {
#include "data/ov006/data_ov006_0236a500.inc.c"
};
u32 data_ov006_0236a5b8[2] = {
#include "data/ov006/data_ov006_0236a5b8.inc.c"
};
u32 data_ov006_0236a4f0[2] = {
#include "data/ov006/data_ov006_0236a4f0.inc.c"
};
u32 data_ov006_0236a4e8[2] = {
#include "data/ov006/data_ov006_0236a4e8.inc.c"
};
u32 data_ov006_0236a4e0[2] = {
#include "data/ov006/data_ov006_0236a4e0.inc.c"
};
u32 data_ov006_0236a6b8[2] = {
#include "data/ov006/data_ov006_0236a6b8.inc.c"
};
u32 data_ov006_0236a4d0[2] = {
#include "data/ov006/data_ov006_0236a4d0.inc.c"
};
u32 data_ov006_0236a788[2] = {
#include "data/ov006/data_ov006_0236a788.inc.c"
};
static SlotSprites data_ov006_0236a790 = {
#include "data/ov006/data_ov006_0236a790.inc.c"
};
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
