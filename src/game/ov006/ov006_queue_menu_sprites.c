#include "types.h"

void Sprite_Queue(int list, int x, int y, const void* data, int attr, int tile);

void MenuDepth_Push(void);
void func_ov006_02350ee8(void);
void func_ov006_02354778(void);
void Ov006_UpdateTextPages(void);
void MenuDepth_Pop(void);

static u8 data_ov006_0236c040[] = {
#include "data/ov006/data_ov006_0236c040.inc.c"
};
static u8 data_ov006_0236c070[] = {
#include "data/ov006/data_ov006_0236c070.inc.c"
};
static u8 data_ov006_0236c068[] = {
#include "data/ov006/data_ov006_0236c068.inc.c"
};
static u8 data_ov006_0236c060[] = {
#include "data/ov006/data_ov006_0236c060.inc.c"
};
static u8 data_ov006_0236c058[] = {
#include "data/ov006/data_ov006_0236c058.inc.c"
};
static u8 data_ov006_0236c050[] = {
#include "data/ov006/data_ov006_0236c050.inc.c"
};
static u8 data_ov006_0236c048[] = {
#include "data/ov006/data_ov006_0236c048.inc.c"
};
u32 data_ov006_0236c078[] = {
#include "data/ov006/data_ov006_0236c078.inc.c"
};
/* declared `u8 data_ov006_0236c088[]` in func_ov006_023586e0.c: relocation at data_ov006_0236c088+0x0 is not on a 4-byte field */
u32 data_ov006_0236c088[] = {
#include "data/ov006/data_ov006_0236c088.inc.c"
};

/* Queues seven sprites on list 0: four at y 0x458 (x 32, 80, 128, 176) and three at
 * y 0x488 (x 32, 80, 128). */
void Ov006_QueueMenuSprites(void) {
  Sprite_Queue(0, 32, 0x458, data_ov006_0236c040, 0, 0);
  Sprite_Queue(0, 80, 0x458, data_ov006_0236c070, 0, 0);
  Sprite_Queue(0, 128, 0x458, data_ov006_0236c068, 0, 0);
  Sprite_Queue(0, 176, 0x458, data_ov006_0236c060, 0, 0);
  Sprite_Queue(0, 32, 0x488, data_ov006_0236c058, 0, 0);
  Sprite_Queue(0, 80, 0x488, data_ov006_0236c050, 0, 0);
  Sprite_Queue(0, 128, 0x488, data_ov006_0236c048, 0, 0);
}
