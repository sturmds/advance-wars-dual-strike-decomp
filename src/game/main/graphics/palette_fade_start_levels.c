#include "types.h"
#include "game/palette_fade.h"

Task* PaletteFade_StartLevels(void* parentOrList, int duration, int firstSlot,
                              int slotCount, int fromLevel, int toLevel) {
  return PaletteFade_Start(parentOrList, 1, NULL, 0, duration, firstSlot,
                           slotCount, fromLevel, toLevel);
}
