#include "types.h"

void UnitIcon_DrawEntries(u16 *entry, int type, int look, u8 carrying, u8 capture, u16 shownHp, u16 dived, u8 hpHidden,
                   BOOL lowFuel, BOOL lowAmmo, BOOL paralysed);

/* Draws a unit icon at tile (x, y) of the 32-tile-wide map through
 * UnitIcon_DrawEntries, with its HP, dive and status arguments all zero. */
void UnitIcon_Draw(u16 *map, int x, int y, int unitType, u16 look, u8 carrying, u8 capture)
{
    UnitIcon_DrawEntries(map + x + y * 32, unitType, look, carrying, capture, 0, 0, 0, 0, 0, 0);
}
