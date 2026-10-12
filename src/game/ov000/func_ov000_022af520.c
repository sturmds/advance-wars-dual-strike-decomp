#include "types.h"
#include "game/map_unit.h"

u32* MapScript_Get(void);
int EventTriggers_Run(u32 script, MapUnit* unit, int a2, int a3);

int func_ov000_022af520(void) {
  u32* script = MapScript_Get();

  if (script != NULL && script[1] != 0) {
    return EventTriggers_Run(script[1], NULL, 0, 1);
  }
  return 0;
}
