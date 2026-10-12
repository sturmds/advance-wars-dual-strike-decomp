#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern u8 data_021398d0[];

void Gfx_ClearLayerMap(int layer);
void MapView_DrawRangeTile(s16 x, s16 y);
void Gfx_RequestLayerCopy(int layer);
s16 TransferQueue_Add(const void* src, void* dst, u32 size);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Task step: clears layer 0 and draws the range highlight for the 16 by 12
 * tiles of the view (13 rows when the map goes on below), then queues 0x80
 * bytes from data_021398d0 into the layer 0 data and sets task->unk_66 to 2. */
void MapView_DrawAllRangeTiles(Task* task) {
  int rows;
  int col;
  Unk_02080440_1c4* map;
  int row;
  int left;
  int top;
  int y;
  map = GAME->unk_1c4;
  left = map->scrollX >> 4;
  top = map->scrollY >> 4;

  rows = (map->mapHeight - top > 12) ? 13 : 12;
  Gfx_ClearLayerMap(0);
  for (row = 0; row < rows; row++) {
    y = top + row;
    for (col = 0; col < 16; col++) {
      MapView_DrawRangeTile(left + col, y);
    }
  }
  Gfx_RequestLayerCopy(0);
  TransferQueue_Add(data_021398d0, data_027c0280->layer0Map + 0x3ae0, 0x80);
  task->unk_66 = 2;
}
