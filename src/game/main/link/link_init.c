#include "types.h"
#include "game/Unk_0215fe8c.h"

extern Unk_0215fe8c* data_0215fe8c;

void Wireless_Init(void);
void func_020fcaf4(void);

/* Called once by main at start-up. */
void Link_Init(void) {
  Wireless_Init();
  func_020fcaf4();
  data_0215fe8c->linkActive = 0;
}
