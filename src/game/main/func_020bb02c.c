#include "types.h"
#include "game/ai_reach_tile.h"
#include "game/unit_target_entry.h"

extern void* data_02168314;
extern UnitTargetEntry* data_022a6dcc;
extern AiReachTile* data_022a6dd0;
void TileCodeTable_Load(void*, void*, void*);

void func_020bb02c(void) {
  void* t0 = data_02168314;
  data_022a6dcc = t0;
  data_022a6dd0 = t0;
  TileCodeTable_Load(&data_022a6dd0, &data_022a6dcc, t0);
}
