#include "types.h"

typedef struct Unk_0207a148_0 Unk_0207a148_0;

struct Unk_0207a148_0 {
  u8 pad_00[0x1c];
  int childrenChanged;
};

extern Unk_0207a148_0* data_0215fe84;

/* TRUE once after the set of connected children changed; clears the flag. */
BOOL Wireless_TakeChildrenChanged(void) {
  Unk_0207a148_0* state = data_0215fe84;
  if (state->childrenChanged == 1) {
    state->childrenChanged = 0;
    return TRUE;
  }
  return FALSE;
}
