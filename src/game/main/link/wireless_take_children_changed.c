#include "types.h"
#include "game/Unk_0215fe84.h"

extern Unk_0215fe84* data_0215fe84;

/* TRUE once after the set of connected children changed; clears the flag. */
BOOL Wireless_TakeChildrenChanged(void) {
  Unk_0215fe84* state = data_0215fe84;
  if (state->childrenChanged == 1) {
    state->childrenChanged = 0;
    return TRUE;
  }
  return FALSE;
}
