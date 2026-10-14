#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/screen_contexts.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
/* Queues sprite `index` of a loaded sprite file at (x, y); 0 if the file
 * has no such sprite. */
BOOL SpriteFile_Queue(void* set, int index, int arg, int x, int y, int attr,
                      int tile, int list);

extern Unk_ov006_0235f690* data_ov006_0236d640;
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
extern u8 data_0217fb08[];

u8 data_ov006_0236a9e8[] = {
#include "data/ov006/data_ov006_0236a9e8.inc.c"
};
static u8 data_ov006_0236a9e0[] = {
#include "data/ov006/data_ov006_0236a9e0.inc.c"
};
static u8 data_ov006_0236a9d8[] = {
#include "data/ov006/data_ov006_0236a9d8.inc.c"
};
static u8 data_ov006_0236aa68[] = {
#include "data/ov006/data_ov006_0236aa68.inc.c"
};
u32 data_ov006_0236aa60[] = {
#include "data/ov006/data_ov006_0236aa60.inc.c"
};
u32 data_ov006_0236aa58[] = {
#include "data/ov006/data_ov006_0236aa58.inc.c"
};
u32 data_ov006_0236aa50[] = {
#include "data/ov006/data_ov006_0236aa50.inc.c"
};
u32 data_ov006_0236aa48[] = {
#include "data/ov006/data_ov006_0236aa48.inc.c"
};
u32 data_ov006_0236aa40[] = {
#include "data/ov006/data_ov006_0236aa40.inc.c"
};
u32 data_ov006_0236aa38[] = {
#include "data/ov006/data_ov006_0236aa38.inc.c"
};
u8 data_ov006_0236aa30[] = {
#include "data/ov006/data_ov006_0236aa30.inc.c"
};
u8 data_ov006_0236aa28[] = {
#include "data/ov006/data_ov006_0236aa28.inc.c"
};
u32 data_ov006_0236aa20[] = {
#include "data/ov006/data_ov006_0236aa20.inc.c"
};
u8 data_ov006_0236aa18[] = {
#include "data/ov006/data_ov006_0236aa18.inc.c"
};
u32 data_ov006_0236aa10[] = {
#include "data/ov006/data_ov006_0236aa10.inc.c"
};
u32 data_ov006_0236aa08[] = {
#include "data/ov006/data_ov006_0236aa08.inc.c"
};
u8 data_ov006_0236aa00[] = {
#include "data/ov006/data_ov006_0236aa00.inc.c"
};
static u8 data_ov006_0236a9f8[] = {
#include "data/ov006/data_ov006_0236a9f8.inc.c"
};
u8 data_ov006_0236a9f0[] = {
#include "data/ov006/data_ov006_0236a9f0.inc.c"
};
/* declared `RowShapes data_ov006_0236aa70` in ov006_draw_property_counts.c: declared in src/game/ov006/ov006_draw_property_counts.c, with a type no header defines */
void* data_ov006_0236aa70[] = {0, &data_ov006_0236aa20, &data_ov006_0236aa60,
                               &data_ov006_0236aa10, &data_ov006_0236aa08};
/* declared `ColumnShapes data_ov006_0236aa84` in ov006_draw_property_counts.c: declared in src/game/ov006/ov006_draw_property_counts.c, with a type no header defines */
void* data_ov006_0236aa84[] = {0,
                               &data_ov006_0236aa58,
                               &data_ov006_0236aa50,
                               &data_ov006_0236aa48,
                               &data_ov006_0236aa40,
                               &data_ov006_0236aa38};
/* declared `TileGrid data_ov006_0236aa9c` in ov006_draw_property_counts.c: declared in src/game/ov006/ov006_draw_property_counts.c, with a type no header defines */
u32 data_ov006_0236aa9c[] = {
#include "data/ov006/data_ov006_0236aa9c.inc.c"
};
/* declared `u8 data_ov006_0236aad8[]` in ov006_load_mapedit_graphics.c: relocation at data_ov006_0236aad8+0xc is not on a 4-byte field */
u32 data_ov006_0236aad8[] = {
#include "data/ov006/data_ov006_0236aad8.inc.c"
};

/* Queues the fixed sprites of the second screen: sprites 0 and 1 of /mapedit
 * file 00a (unk_68), each with two more sprites at fixed places. Called
 * by Ov006_UpdateSubScreen. */
void Ov006_QueueScreen1Sprites(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  SpriteFile_Queue(data_ov006_0236d640->unk_68, 0, 0, 88, 166, 0xf2c7, 0, 0);
  Sprite_Queue(0, 80, 136, data_ov006_0236a9d8, 0x3000, 0);
  Sprite_Queue(0, 72, 169, data_ov006_0236a9f8, 0xf000, 0);
  SpriteFile_Queue(data_ov006_0236d640->unk_68, 1, 0, 48, 166, 0xf2c7, 0, 0);
  Sprite_Queue(0, 40, 150, data_ov006_0236a9e0, 0x2000, 0);
  Sprite_Queue(0, 32, 169, data_ov006_0236aa68, 0xf000, 0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
