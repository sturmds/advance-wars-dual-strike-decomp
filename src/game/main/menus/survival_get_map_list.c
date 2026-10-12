#include "types.h"

extern u16 gSurvivalMapsMode0[];
extern u16 gSurvivalMapsMode1[];
extern u16 gSurvivalMapsMode2[];
extern u16 gSurvivalMapsMode3[];
extern u16 gSurvivalMapsMode4[];
extern u16 gSurvivalMapsMode5[];

/* The map list of a Survival mode variant (0-5): map ids (indexes into
 * Ov000_gMapHeaders) ending with 0. NULL for other modes. */
u16* Survival_GetMapList(int mode) {
  switch (mode) {
    case 0:
      return gSurvivalMapsMode0;
    case 1:
      return gSurvivalMapsMode1;
    case 2:
      return gSurvivalMapsMode2;
    case 3:
      return gSurvivalMapsMode3;
    case 4:
      return gSurvivalMapsMode4;
    case 5:
      return gSurvivalMapsMode5;
    default:
      return NULL;
  }
}
