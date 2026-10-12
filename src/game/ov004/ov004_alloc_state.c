#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern void* data_ov004_022bf14c;
extern void* data_ov004_022bf150;
extern void* data_ov004_022bf154;
extern u8 data_ov004_022bf15c[];
extern u8 data_ov004_022c395c[];
extern u8 data_ov004_022d395c[];

void* Mem_Alloc(u32 size);

void Ov004_AllocState(Ov004Task* task) {
  task->state = Mem_Alloc(sizeof(Unk_ov004_022bf158));
  data_ov004_022bf158 = task->state;
  data_ov004_022bf14c = data_ov004_022d395c;
  data_ov004_022bf150 = data_ov004_022c395c;
  data_ov004_022bf154 = data_ov004_022bf15c;
}
