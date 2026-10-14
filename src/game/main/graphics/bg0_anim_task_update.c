#include "types.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
const u8 data_02139810[] = {
#include "data/main/data_02139810.inc.c"
};
/* 0x80-byte frames of 4 tiles each */
const u8 data_021398d0[] = {
#include "data/main/data_021398d0.inc.c"
};
extern u16 data_02139d10
    []; /* colour cycles: 15 colours, repeated so any start reads 15 */
extern u16 data_02139d50[];

s16 TransferQueue_Add(const void* src, void* dst, u32 size);
int MainLoop_GetFrameCount(void);
void func_02003154(const void* src, int offset, u32 size);

/* Update of the task in the script data_02169dd8 (MapView_DrawAllRangeTiles starts
 * it). Until frame 8 it copies the next 4-tile frame into the layer 0 tile
 * data, once per call. Every call it copies 15 colours of a cycle into
 * palette colours 1 to 15, one step every 4 frames; task->unk_64 picks the
 * cycle. */
void Bg0AnimTask_Update(Task* task) {
  if (task->unk_66 < 8) {
    TransferQueue_Add(data_021398d0 + (task->unk_66 << 7),
                      data_027c0280->unk_e4 + 0x3ae0, 0x80);
    task->unk_66++;
  }
  if (task->unk_64 == 0) {
    func_02003154(&data_02139d50[(MainLoop_GetFrameCount() >> 2) % 15] + 1, 2,
                  30);
  } else {
    func_02003154(&data_02139d10[(MainLoop_GetFrameCount() >> 2) % 15] + 1, 2,
                  30);
  }
}
