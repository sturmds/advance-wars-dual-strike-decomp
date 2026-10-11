#include "types.h"
#include "game/map_unit.h"
#include "game/screen_other_map.h"

MapUnit *Unit_FindFreeSlot(int player);
void func_020c73a8(int player);
void MapUnit_ResetAiState(MapUnit *unit);
u8 func_020c7fb0(MapUnit *unit, int x, int y);
void func_020c7398(int unitIndex);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Two-map battles: moves unit to tile (x, y) of the other map. A free slot of
 * the current player is taken on the other map (func_020c73a8 counts the
 * unit there), the unit is copied in, ready to act again, with its cargo
 * copied by func_020c7fb0, then the unit is removed from this map. */
void Unit_TransferToOtherMap(MapUnit *unit, int x, int y)
{
    int player;
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedState;
    MapUnit *copy;
    int i;

    player = STATE->currentPlayer;
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    savedState = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    copy = Unit_FindFreeSlot(player);
    func_020c73a8(player);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    *copy = *unit;
    copy->x = x;
    copy->y = y;
    copy->flags &= ~MAP_UNIT_WAITED;
    MapUnit_ResetAiState(copy);
    for (i = 0; i < 2; i++) {
        if (unit->cargo[i] != 0) {
            copy->cargo[i] = func_020c7fb0(&STATE->units[unit->cargo[i]], x, y);
        }
    }
    func_020c7398(unit - STATE->units);
}
