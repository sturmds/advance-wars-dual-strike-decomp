#include "types.h"
#include "game/unlocks.h"

void GameFlagsLow_Set(int id, int value);
void GameFlagsMid_Set(u32 id, u8 value);
void GameFlagsHigh_Set(u32 id, u8 value);

void GameFlag_Set(int id, int value) {
  if (id >= 95 && id < 159) {
    GameFlagsHigh_Set(id - 95, value);
    return;
  }
  if (id >= 32 && id < 95) {
    GameFlagsMid_Set(id - 32, value);
    return;
  }
  if (id >= 0 && id < 32) {
    GameFlagsLow_Set(id, value);
  }
}
