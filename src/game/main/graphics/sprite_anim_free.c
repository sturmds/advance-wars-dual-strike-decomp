#include "types.h"

/* Clears the first word of a handle, the flag that SpriteAnim_Advance and
 * SpriteAnim_Draw test before they use it. A null handle is ignored.
 * The compiler needs the flag test as a separate step to match. */
void SpriteAnim_Free(u32* handle) {
  BOOL active;

  if (handle != NULL && *handle != 0) {
    active = TRUE;
  } else {
    active = FALSE;
  }
  if (active) {
    *handle = 0;
  }
}
