#include "types.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];

void func_020ddb38(void);

/* Event script command: runs func_020ddb38 (rolls the next turn's weather,
 * saves the RNG state, refreshes the units), goes on to the next record and
 * ends this frame's script run. */
BOOL EventCmd_RollWeatherAndRefreshUnits(s16 index) {
  func_020ddb38();
  data_022977f4[index].unk_00++;
  return FALSE;
}
