#include "types.h"
#include "game/BgControl.h"

/* offset is in bytes; BGxCNT stores the screen base in 2 KB units. */
void BgControl_SetScreenBase(BgControl* bg, u32 offset) {
  bg->screenBase = offset >> 11;
}
