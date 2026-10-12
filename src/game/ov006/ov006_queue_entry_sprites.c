#include "types.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/unlocks.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_ov006_0236a810[][2];
extern u8 data_ov006_0236a844[][2];
extern void* data_ov006_0236a908[];
extern void* data_ov006_0236a970[];

/* Queues two sprites for each entry of the list that Ov006_UpdateEntryView
 * indexes (26 entries with game flag 0x2b set, else 25). Each entry's (x, y)
 * comes from a byte-pair table; the second sprite sits 8 pixels left and 17
 * down. Flags 0xc and 0x8 of unk_02 set bit 8 of the y word (OBJ attribute 0:
 * affine) for the first and second sprite. */
void Ov006_QueueEntrySprites(void) {
  u16 flags = data_ov006_0236d640->unk_02;
  BOOL firstAffine = (flags & 0xc) ? 1 : 0;
  BOOL secondAffine = (flags & 8) ? 1 : 0;
  u8(*pos)[2];
  int i;
  int count;

  if (GameFlag_IsSet(0x2b) == 1) {
    pos = data_ov006_0236a844;
    count = 26;
  } else {
    pos = data_ov006_0236a810;
    count = 25;
  }
  /* needed to match: the original tests count twice before the loop */
  i = 0;
  if (count <= 0) {
    return;
  }
  for (; i < count; i++) {
    u8 x = (*pos)[0];
    u8 y = (*pos)[1];
    pos++;
    Sprite_Queue(0, x & 0x1ff, (y & 0xff) | 0x400 | (firstAffine ? 0x100 : 0),
                 data_ov006_0236a908[i], 0x6000, 0);
    Sprite_Queue(0, (x - 8) & 0x1ff,
                 ((y + 17) & 0xff) | 0x400 | (secondAffine ? 0x100 : 0),
                 data_ov006_0236a970[i], 0xf000, 0);
  }
}
