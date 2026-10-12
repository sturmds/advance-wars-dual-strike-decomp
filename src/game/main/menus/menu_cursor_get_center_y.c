#include "types.h"
#include "game/menu_button.h"

/* Screen y halfway between the cursor's top and bottom corners. */
s16 MenuCursor_GetCenterY(void) {
  return (gMenuCursor->corners[0][1] + gMenuCursor->corners[3][1]) >> 1;
}
