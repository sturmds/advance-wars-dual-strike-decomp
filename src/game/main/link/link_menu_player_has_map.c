#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int DesignMap_IsUsed(DesignMap* row);

/* Whether any of the three map slots of a link player holds a map. */
int LinkMenu_PlayerHasMap(int player) {
  DesignMap* row = &LINK_MAP_ROWS[player * 3];
  int i;

  for (i = 0; i < 3; i++, row++) {
    if (DesignMap_IsUsed(row) == 1) {
      return 1;
    }
  }
  return 0;
}
