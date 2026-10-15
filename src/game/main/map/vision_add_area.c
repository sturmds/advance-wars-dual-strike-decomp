#include "types.h"
#include "game/Unk_02080440.h"
#include "game/vision.h"

/* Adds delta to every tile within range of (x, y) in the grids gridMask picks
 * (see game/vision.h); player is passed on for its skill checks. */
void Vision_AddArea(s16 x, s16 y, s16 range, int gridMask, s8 delta,
                    int player) {
  switch (gridMask) {
    case 0:
      break;
    case 1:
      VisionGrid_AddArea(x, y, range,
                         VISION_GRID(MAP_STATE, MAP_STATE->visionGrid0), delta,
                         player);
      break;
    case 3:
      VisionGrid_AddArea(x, y, range,
                         VISION_GRID(MAP_STATE, MAP_STATE->visionGrid0), delta,
                         player);
    case 2:
      VisionGrid_AddArea(x, y, range,
                         VISION_GRID(MAP_STATE, MAP_STATE->visionGrid1), delta,
                         player);
      break;
  }
}
