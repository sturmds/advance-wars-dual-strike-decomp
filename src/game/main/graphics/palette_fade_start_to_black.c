#include "types.h"
#include "game/palette_fade.h"

Task* PaletteFade_StartToBlack(void* parentOrList, int duration, int firstSlot,
                               int slotCount) {
  return PaletteFade_Start(parentOrList, 1, NULL, 0, duration, firstSlot,
                           slotCount, 0, 16);
}
