#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when one of the player's unit slots 1 to 50 holds a unit. The
 * original keeps the slot and the counter in two registers and multiplies
 * by sizeof(MapUnit) on each pass; ours merges them (89%). */
BOOL func_020c0764(int player)
{
    int count = 1;
    int slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
    MapUnit *units = STATE->units;

    do {
        if (units[slot].type != 0) {
            break;
        }
        count++;
        slot++;
    } while (count < 51);
    return count != 51;
}
