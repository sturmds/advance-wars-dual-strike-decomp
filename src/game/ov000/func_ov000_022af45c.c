#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_ov000_022f6bd8[];

u32* MapScript_Get(void);
int EventTriggers_Run(u32 script, MapUnit* unit, int a2, int a3);
void* Task_Create(const void* script, int list);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* For a unit of player 1 only: runs the map script entry at offset 8 with the
 * unit, and when that returns nonzero starts the task data_ov000_022f6bd8. */
int func_ov000_022af45c(MapUnit* unit) {
  u32* script = MapScript_Get();
  int result;
  int owner = (((unit - STATE->units) & 0xc0) >> 6) + 1;

  if (owner != 1) {
    return 0;
  }
  if (script != NULL && script[2] != 0) {
    result = EventTriggers_Run(script[2], unit, 0, 1);
    if (result != 0) {
      Task_Create(data_ov000_022f6bd8, 3);
    }
    return result;
  }
  return 0;
}
