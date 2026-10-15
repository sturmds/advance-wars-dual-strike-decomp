#include "types.h"
#include "game/Unk_02080440.h"

/* Re-reads each tile byte (terrain and owner) of the unk_608c list from the
 * map, up to the 0xff entry, so owner changes show in the list. */
void MapSiteList_RefreshTiles(void) {
  int i;

  for (i = 0; i < MAP_SITE_COUNT; i++) {
    if (MAP_STATE->unk_608c[i].tile == MAP_SITE_END) {
      return;
    }
    MAP_STATE->unk_608c[i].tile =
        MAP_STATE->mapTiles
            ->tiles[MAP_STATE->unk_608c[i].x +
                    MAP_STATE->mapTiles->rowStart[MAP_STATE->unk_608c[i].y]];
  }
}
