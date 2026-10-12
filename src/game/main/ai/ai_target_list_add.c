#include "types.h"
#include "game/unit_target_entry.h"

extern UnitTargetEntry* data_022a6dcc;
extern int data_022a6dd4;

void AiTargetList_Add(u16 a, u16 b, u16 unitSlot, u16 c) {
  UnitTargetEntry* list = data_022a6dcc;

  list[data_022a6dd4].x = a;
  list[data_022a6dd4].y = b;
  list[data_022a6dd4].unitSlot = unitSlot;
  list[data_022a6dd4].value = c;
  data_022a6dd4++;
}
