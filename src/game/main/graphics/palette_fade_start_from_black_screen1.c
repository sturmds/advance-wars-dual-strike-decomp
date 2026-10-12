#include "types.h"
#include "game/palette_fade.h"

Task* PaletteFade_StartFromBlackScreen1(void* parentOrList,
                                        const void* palettes, int duration,
                                        int firstSlot, int slotCount) {
  return PaletteFade_Start(parentOrList, 0, palettes, 1, duration, firstSlot,
                           slotCount, 16, 0);
}
