#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c* data_0215fe8c;

void Link_ParentHandlePackets(void);
void Link_ChildHandlePackets(void);

/* In link mode, runs both packet handlers. */
void Link_HandlePackets(void) {
  if (data_0215fe8c->linkActive == 0) {
    return;
  }
  Link_ParentHandlePackets();
  Link_ChildHandlePackets();
}
