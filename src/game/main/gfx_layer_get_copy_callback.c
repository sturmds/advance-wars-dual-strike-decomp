#include "types.h"

typedef void (*LayerCopyFunc)(void);

void GfxLayer3_RequestCopy(void);
void GfxLayer2_RequestCopy(void);
void GfxLayer1_RequestCopy(void);
void GfxLayer0_RequestCopy(void);

/* Returns the function that queues a tile map copy for layer 0-3;
 * anything else gives layer 0's. */
LayerCopyFunc GfxLayer_GetCopyCallback(int layer) {
  switch (layer) {
    default:
    case 0:
      return GfxLayer0_RequestCopy;
    case 1:
      return GfxLayer1_RequestCopy;
    case 2:
      return GfxLayer2_RequestCopy;
    case 3:
      return GfxLayer3_RequestCopy;
  }
}
