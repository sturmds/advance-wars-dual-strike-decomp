#include "types.h"
#include "game/map_script.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_ov000_022f6bd8[];

MapScript* MapScript_Get(void);
int EventTriggers_Run(const EventTrigger* list, MapUnit* unit, int id,
                      int flag);
void* Task_Create(const void* script, int list);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* For a unit of player 1 only: runs the map's list 08 (see MapScript) with
 * the unit, and when that returns nonzero starts the task
 * data_ov000_022f6bd8. */
int MapScript_RunList08(MapUnit* unit) {
  MapScript* script = MapScript_Get();
  int result;
  int owner = (((unit - STATE->units) & 0xc0) >> 6) + 1;

  if (owner != 1) {
    return 0;
  }
  if (script != NULL && script->list08 != NULL) {
    result = EventTriggers_Run(script->list08, unit, 0, 1);
    if (result != 0) {
      Task_Create(data_ov000_022f6bd8, 3);
    }
    return result;
  }
  return 0;
}
