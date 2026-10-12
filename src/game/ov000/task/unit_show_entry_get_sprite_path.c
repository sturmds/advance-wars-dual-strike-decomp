#include "types.h"
#include "game/screen_other_map.h"
#include "game/unit_show_entry.h"

const char* UnitShowEntry_GetSpritePath(int entry) {
  if (data_027c027c == data_027c0288.unk_00) {
    return gUnitShowEntries[entry].mainSpritePath;
  }
  return gUnitShowEntries[entry].subSpritePath;
}
