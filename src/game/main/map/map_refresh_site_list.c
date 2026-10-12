#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Re-reads each tile byte (terrain and owner) of the unk_608c list from the
 * map, up to the 0xff entry, so owner changes show in the list. */
void MapSiteList_RefreshTiles(void) {
  int i;

  for (i = 0; i < 92; i++) {
    if (STATE->unk_608c[i].tile == 0xff) {
      return;
    }
    STATE->unk_608c[i].tile =
        STATE->mapTiles->tiles[STATE->unk_608c[i].x +
                               STATE->mapTiles->rowStart[STATE->unk_608c[i].y]];
  }
}
