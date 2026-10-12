#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Tile column of a player slot on the Vs setup screen: 8 tiles per slot,
 * shifted by 4 with three slots and by 8 with two (setup.unk_2e3 counts the
 * slots). */
int VsSetup_GetSlotTileX(int slot) {
  switch (gMenuState.setup.unk_2e3) {
    case 4:
      return slot * 8;
    case 3:
      return slot * 8 + 4;
    default:
      return slot * 8 + 8;
  }
}
