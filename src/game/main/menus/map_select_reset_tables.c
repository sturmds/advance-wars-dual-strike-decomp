#include "types.h"
#include "game/map_select.h"

/* Clears the map select tables (MapListTables): every tab's list index and
 * the page and page position of each entry, plus subStrip. */
void MapSelect_ResetTables(void) {
  int entry;
  int page;
  int tab;

  TABLES.subStrip = 0;
  for (tab = 0; tab < (int)ARRAY_COUNT(TABLES.listOfTab); tab++) {
    TABLES.listOfTab[tab] = 0;
    for (entry = 0; entry < (int)ARRAY_COUNT(TABLES.unk_0d0[0]); entry++) {
      for (page = 0; page < (int)ARRAY_COUNT(TABLES.unk_000[0][0]); page++) {
        TABLES.unk_000[tab][entry][page] = 0;
      }
      TABLES.unk_0d0[tab][entry] = 0;
    }
  }
}
