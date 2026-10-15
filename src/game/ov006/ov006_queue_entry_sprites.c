#include "types.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/unlocks.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);

extern u32 data_ov006_0236a528[];
extern u32 data_ov006_0236a558[];
extern u32 data_ov006_0236a6c8[];
extern u32 data_ov006_0236a758[];
extern u32 data_ov006_0236a580[];
extern u32 data_ov006_0236a5f0[];
extern u32 data_ov006_0236a6c0[];
extern u32 data_ov006_0236a750[];
extern u32 data_ov006_0236a4d8[];
extern u32 data_ov006_0236a500[];
extern u32 data_ov006_0236a5e8[];
extern u32 data_ov006_0236a748[];
extern u32 data_ov006_0236a5b8[];
extern u32 data_ov006_0236a5e0[];
extern u32 data_ov006_0236a6b0[];
extern u32 data_ov006_0236a740[];
extern u32 data_ov006_0236a4f0[];
extern u32 data_ov006_0236a5d8[];
extern u32 data_ov006_0236a5f8[];
extern u32 data_ov006_0236a6a8[];
extern u32 data_ov006_0236a518[];
extern u32 data_ov006_0236a5d0[];
extern u32 data_ov006_0236a6a0[];
extern u32 data_ov006_0236a730[];
extern u32 data_ov006_0236a520[];
extern u32 data_ov006_0236a5c8[];
extern u32 data_ov006_0236a698[];
extern u32 data_ov006_0236a728[];
extern u32 data_ov006_0236a5c0[];
extern u32 data_ov006_0236a690[];
extern u32 data_ov006_0236a6b8[];
extern u32 data_ov006_0236a720[];
extern u32 data_ov006_0236a4e0[];
extern u32 data_ov006_0236a4f8[];
extern u32 data_ov006_0236a688[];
extern u32 data_ov006_0236a718[];
extern u32 data_ov006_0236a5b0[];
extern u32 data_ov006_0236a680[];
extern u32 data_ov006_0236a710[];
extern u32 data_ov006_0236a738[];
extern u32 data_ov006_0236a4e8[];
extern u32 data_ov006_0236a5a8[];
extern u32 data_ov006_0236a678[];
extern u32 data_ov006_0236a708[];
extern u32 data_ov006_0236a4d0[];
extern u32 data_ov006_0236a5a0[];
extern u32 data_ov006_0236a670[];
extern u32 data_ov006_0236a700[];
extern u32 data_ov006_0236a598[];
extern u32 data_ov006_0236a668[];
extern u32 data_ov006_0236a6f8[];
extern u32 data_ov006_0236a788[];
extern u32 data_ov006_0236a590[];
extern u32 data_ov006_0236a660[];
extern u32 data_ov006_0236a6f0[];
extern u32 data_ov006_0236a780[];
extern u32 data_ov006_0236a510[];
extern u32 data_ov006_0236a658[];
extern u32 data_ov006_0236a6e8[];
extern u32 data_ov006_0236a778[];
extern u32 data_ov006_0236a588[];
extern u32 data_ov006_0236a650[];
extern u32 data_ov006_0236a6e0[];
extern u32 data_ov006_0236a770[];
extern u32 data_ov006_0236a578[];
extern u32 data_ov006_0236a648[];
extern u32 data_ov006_0236a6d8[];
extern u32 data_ov006_0236a768[];
extern u32 data_ov006_0236a570[];
extern u32 data_ov006_0236a640[];
extern u32 data_ov006_0236a6d0[];
extern u32 data_ov006_0236a760[];
extern u32 data_ov006_0236a568[];
extern u32 data_ov006_0236a638[];
extern u32 data_ov006_0236a560[];
extern u32 data_ov006_0236a630[];
extern u32 data_ov006_0236a508[];
extern u32 data_ov006_0236a628[];
extern u32 data_ov006_0236a550[];
extern u32 data_ov006_0236a620[];
extern u32 data_ov006_0236a548[];
extern u32 data_ov006_0236a618[];
extern u32 data_ov006_0236a540[];
extern u32 data_ov006_0236a610[];
extern u32 data_ov006_0236a538[];
extern u32 data_ov006_0236a608[];
extern u32 data_ov006_0236a530[];
extern u32 data_ov006_0236a600[];

u8 data_ov006_0236a844[][2] = {
#include "data/ov006/data_ov006_0236a844.inc.c"
};
u8 data_ov006_0236a810[][2] = {
#include "data/ov006/data_ov006_0236a810.inc.c"
};
/* declared `void* data_ov006_0236a8c0[ENTRY_COUNT]` in ov006_queue_terrain_entry_sprites.c: declared in src/game/ov006/ov006_queue_terrain_entry_sprites.c, with a type no header defines */
void* data_ov006_0236a8c0[] = {
    &data_ov006_0236a758, &data_ov006_0236a750, &data_ov006_0236a748,
    &data_ov006_0236a740, &data_ov006_0236a5f8, &data_ov006_0236a730,
    &data_ov006_0236a728, &data_ov006_0236a720, &data_ov006_0236a718,
    &data_ov006_0236a710, &data_ov006_0236a708, &data_ov006_0236a700,
    &data_ov006_0236a6f8, &data_ov006_0236a6f0, &data_ov006_0236a6e8,
    &data_ov006_0236a6e0, &data_ov006_0236a6d8, &data_ov006_0236a6d0};
void* data_ov006_0236a878[] = {
    &data_ov006_0236a528, &data_ov006_0236a580, &data_ov006_0236a500,
    &data_ov006_0236a5b8, &data_ov006_0236a4f0, &data_ov006_0236a518,
    &data_ov006_0236a520, &data_ov006_0236a6b8, &data_ov006_0236a4e0,
    &data_ov006_0236a738, &data_ov006_0236a4e8, &data_ov006_0236a4d0,
    &data_ov006_0236a788, &data_ov006_0236a780, &data_ov006_0236a778,
    &data_ov006_0236a770, &data_ov006_0236a768, &data_ov006_0236a760};
static void* data_ov006_0236a908[] = {
    &data_ov006_0236a6c8, &data_ov006_0236a6c0, &data_ov006_0236a4d8,
    &data_ov006_0236a6b0, &data_ov006_0236a6a8, &data_ov006_0236a6a0,
    &data_ov006_0236a698, &data_ov006_0236a690, &data_ov006_0236a688,
    &data_ov006_0236a680, &data_ov006_0236a678, &data_ov006_0236a670,
    &data_ov006_0236a668, &data_ov006_0236a660, &data_ov006_0236a510,
    &data_ov006_0236a588, &data_ov006_0236a648, &data_ov006_0236a640,
    &data_ov006_0236a638, &data_ov006_0236a630, &data_ov006_0236a628,
    &data_ov006_0236a620, &data_ov006_0236a618, &data_ov006_0236a610,
    &data_ov006_0236a608, &data_ov006_0236a600};
static void* data_ov006_0236a970[] = {
    &data_ov006_0236a558, &data_ov006_0236a5f0, &data_ov006_0236a5e8,
    &data_ov006_0236a5e0, &data_ov006_0236a5d8, &data_ov006_0236a5d0,
    &data_ov006_0236a5c8, &data_ov006_0236a5c0, &data_ov006_0236a4f8,
    &data_ov006_0236a5b0, &data_ov006_0236a5a8, &data_ov006_0236a5a0,
    &data_ov006_0236a598, &data_ov006_0236a590, &data_ov006_0236a658,
    &data_ov006_0236a650, &data_ov006_0236a578, &data_ov006_0236a570,
    &data_ov006_0236a568, &data_ov006_0236a560, &data_ov006_0236a508,
    &data_ov006_0236a550, &data_ov006_0236a548, &data_ov006_0236a540,
    &data_ov006_0236a538, &data_ov006_0236a530};

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
