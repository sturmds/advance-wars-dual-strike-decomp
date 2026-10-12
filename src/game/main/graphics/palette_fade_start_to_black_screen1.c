#include "types.h"
#include "game/palette_fade.h"

Task* PaletteFade_StartToBlackScreen1(void* parentOrList, int duration,
                                      int firstSlot, int slotCount) {
  return PaletteFade_Start(parentOrList, 1, NULL, 1, duration, firstSlot,
                           slotCount, 0, 16);
}
