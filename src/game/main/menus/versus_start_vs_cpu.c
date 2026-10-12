#include "types.h"
#include "game/screen_other_map.h"

void Versus_Start(void);

/* Slot 1 is human and slots 2 to playerCount are AI; the rest are empty.
 * Then starts Versus mode. */
void Versus_StartVsCpu(u8 playerCount) {
  u8 i;

  for (i = 1; i < 5; i++) {
    data_027c027c->unk_48[data_027c0284]->slotControllers[i] =
        PLAYER_CONTROLLER_NONE;
  }
  for (i = 1; i <= playerCount; i++) {
    if (i == 1) {
      data_027c027c->unk_48[data_027c0284]->slotControllers[i] =
          PLAYER_CONTROLLER_HUMAN;
    } else {
      data_027c027c->unk_48[data_027c0284]->slotControllers[i] =
          PLAYER_CONTROLLER_AI;
    }
  }
  Versus_Start();
}
