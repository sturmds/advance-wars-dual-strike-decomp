#include "types.h"
#include "game/Unk_ov004_022bf158.h"

/* also declared in src/game/ov004/ov004_task_reset_state.c */
void* data_ov004_022bf14c;
/* also declared in src/game/ov004/func_ov004_022ade44.c and 84 more */
Unk_ov004_022bf158* data_ov004_022bf158;
/* also declared in src/game/ov004/ov004_load_bg3_from_resource.c and 3 more */
void* data_ov004_022bf150;
/* also declared in src/game/ov004/ov004_init_tile_map.c */
void* data_ov004_022bf154;
static u8 data_ov004_022bf15c[0x4800];  /* size: up to the next symbol */
static u8 data_ov004_022c395c[0x10000]; /* size: up to the next symbol */
static u8 data_ov004_022d395c[0x80004]; /* size: up to the next symbol */

void* Mem_Alloc(u32 size);

void Ov004_AllocState(Ov004Task* task) {
  task->state = Mem_Alloc(sizeof(Unk_ov004_022bf158));
  data_ov004_022bf158 = task->state;
  data_ov004_022bf14c = data_ov004_022d395c;
  data_ov004_022bf150 = data_ov004_022c395c;
  data_ov004_022bf154 = data_ov004_022bf15c;
}
