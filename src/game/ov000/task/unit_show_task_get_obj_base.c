#include "types.h"
#include "game/Unk_022a7c5c.h"

u32 UnitShowTask_GetObjBase(int slot) {
  if (slot & 1) {
    return data_022a7c5c->unk_5c;
  }
  return data_022a7c5c->unk_60;
}
