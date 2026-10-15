#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_state.h"

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Whether co can go into CO slot coSlot (player * 2 + front or back) on the
 * Vs setup screen. 0xff never can. CO_NONE can only on the back CO, and
 * not in a two-map battle. Any other CO cannot when the player's other CO
 * is the same. */
BOOL VsSetup_CanPickCo(int coSlot, int co) {
  if (co == 0xff) {
    return FALSE;
  }
  if (co == 0) {
    if (MAP_STATE->twoMaps == 0 && (coSlot & 1)) {
      return TRUE;
    }
    return FALSE;
  }
  return co != MENU->setup.cos[coSlot >> 1][(coSlot + 1) & 1];
}
