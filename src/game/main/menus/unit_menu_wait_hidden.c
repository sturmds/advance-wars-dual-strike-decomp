#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test: 1 hides the entry. Wait is hidden while another unit
 * stands on the destination tile. */
s8 UnitMenu_WaitHidden(void) {
  return STATE->mapTiles
             ->unitAt[STATE->actingUnitDestX +
                      STATE->mapTiles->rowStart[STATE->actingUnitDestY]] != 0;
}
