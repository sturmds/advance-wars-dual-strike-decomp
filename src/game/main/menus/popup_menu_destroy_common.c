#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

extern Unk_027c0280* data_027c0280;
extern s16 data_0229eae0[];
extern u16 data_0229ead8[];

void func_020c436c(s16* tile, u16* pixels);
void func_0201dc64(Task* task);
void Layer0_ClearMap(void);
void TransferQueue_Add(const void* src, void* dst, u32 size);

/* Common part of both pop-up menu onDestroy functions: sets data_0229ead8 to the tile pair
 * data_0229eae0 times 16, closes all menu buttons, clears the layer 0 tile map and queues its
 * copy to VRAM. The call to func_0201dc64 passes the task on, though that function takes no
 * argument in our C. */
void PopupMenu_DestroyCommon(Task* task) {
  func_020c436c(data_0229eae0, data_0229ead8);
  func_0201dc64(task);
  Layer0_ClearMap();
  TransferQueue_Add(data_027c0280->layer0Map, data_027c0280->unk_e8, 0x800);
}
