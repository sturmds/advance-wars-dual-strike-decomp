#include "types.h"
#include "game/menu_state.h"

/* Tile column of a slot's panel on the Vs setup screen: 7 tiles per slot,
 * starting at column 1, 5 or 9 for four, three or two slots (setup.unk_2e3). */
int VsSetup_GetPanelTileX(int slot) {
  switch (gMenuState.setup.unk_2e3) {
    case 4:
      return slot * 7 + 1;
    case 3:
      return slot * 7 + 5;
    default:
      return slot * 7 + 9;
  }
}
