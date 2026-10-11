#include "types.h"
#include "game/map_unit.h"
#include "game/screen_other_map.h"

MapUnit *Unit_FindFreeSlot(int player);
void Player_CountArrivingUnit(int player);
void MapUnit_ResetAiState(MapUnit *unit);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Two-map battles: copies unit, and the units it carries, into free slots of
 * the current player on the other map at tile (x, y). Returns the copy's slot
 * in the other map's units. Unit_TransferToOtherMap uses it for cargo. */
int Unit_CopyToOtherMap(MapUnit *unit, int x, int y)
{
    int player;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    MapUnit *copy;
    int i;
    int slot;

    player = STATE->currentPlayer;
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    savedState = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    copy = Unit_FindFreeSlot(player);
    Player_CountArrivingUnit(player);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    *copy = *unit;
    copy->x = x;
    copy->y = y;
    MapUnit_ResetAiState(copy);
    for (i = 0; i < 2; i++) {
        if (unit->cargo[i] != 0) {
            copy->cargo[i] = Unit_CopyToOtherMap(&STATE->units[unit->cargo[i]], x, y);
        }
    }
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    savedState = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    slot = copy - STATE->units;
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    return slot;
}
