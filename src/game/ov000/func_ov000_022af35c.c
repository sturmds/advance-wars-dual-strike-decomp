#include "types.h"
#include "game/map_unit.h"

u32* MapScript_Get(void);
int EventTriggers_Run(u32 script, MapUnit* unit, int a2, int a3);

int func_ov000_022af35c(MapUnit* unit) {
  u32* script = MapScript_Get();

  if (script != NULL && script[3] != 0) {
    return EventTriggers_Run(script[3], unit, 0, 1);
  }
  return 0;
}
