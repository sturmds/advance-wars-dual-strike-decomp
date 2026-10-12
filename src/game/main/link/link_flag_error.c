#include "types.h"
#include "game/Unk_0215fe8c.h"

u32 Link_IsActive(void);
extern Unk_0215fe8c* data_0215fe8c;

void Link_FlagError(void) {
  if (Link_IsActive() == 1) {
    data_0215fe8c->error = 1;
  }
}
