#include "types.h"
#include "sdk/mb.h"

extern DlPlayAdvertProgress data_022ace4c;
extern u32 data_022ace48;

void* func_0211ba5c(void);

/* Empties the game list and sets the beacon to pick its first game. */
void MbBeacon_Init(void) {
  data_022ace4c.gameList = NULL;
  data_022ace4c.currentGame = NULL;
  data_022ace4c.state = DLPLAY_ADVERT_STEP_PICK;
  data_022ace48 = 0;
  func_0211ba5c();
}
