#include "types.h"
#include "game/unit_target_entry.h"

extern UnitTargetEntry *data_022a6dcc;
extern int data_022a6dc8;     /* entries in the list */

/* Appends a target to the fire target list: the tile, the unit slot there
 * (0 for a map object) and the map object kind (0 for a unit). */
void FireTargets_Add(u16 x, u16 y, u16 unitSlot, u16 kind)
{
    UnitTargetEntry *list = data_022a6dcc;

    list[data_022a6dc8].unk_04 = x;
    list[data_022a6dc8].unk_06 = y;
    list[data_022a6dc8].unitSlot = unitSlot;
    list[data_022a6dc8].unk_02 = kind;
    data_022a6dc8++;
}
