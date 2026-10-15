#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_entry.h"
#include "game/visual_mode.h"

s16 UnitShowEntry_GetStepSize(int entry) {
  if (data_027c027c->unk_48[data_027c0284]->visualMode == VISUAL_A) {
    return gUnitShowEntries[entry].stepSize;
  }
  return gUnitShowEntries[entry].stepSize * 2;
}
