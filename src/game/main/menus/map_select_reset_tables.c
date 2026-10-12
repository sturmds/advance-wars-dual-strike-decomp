#include "types.h"
#include "game/map_select.h"

/* Clears the map select tables (MapListTables): every tab's list index and
 * the page and page position of each entry, plus subStrip. */
void MapSelect_ResetTables(void) {
  int entry;
  int page;
  int tab;

  TABLES.subStrip = 0;
  for (tab = 0; tab < 4; tab++) {
    TABLES.listOfTab[tab] = 0;
    for (entry = 0; entry < 13; entry++) {
      for (page = 0; page < 4; page++) {
        TABLES.unk_000[tab][entry][page] = 0;
      }
      TABLES.unk_0d0[tab][entry] = 0;
    }
  }
}
